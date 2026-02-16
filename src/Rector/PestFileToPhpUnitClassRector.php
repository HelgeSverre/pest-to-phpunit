<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Rector;

use HelgeSverre\PestToPhpUnit\Helper\CustomExpectationRegistry;
use HelgeSverre\PestToPhpUnit\Helper\ExpectChainUnwinder;
use HelgeSverre\PestToPhpUnit\Helper\NameHelper;
use HelgeSverre\PestToPhpUnit\Mapping\ExpectationMethodMap;
use HelgeSverre\PestToPhpUnit\Mapping\HookMap;
use PhpParser\Comment;
use PhpParser\Comment\Doc;
use PhpParser\Node;
use PhpParser\Node\Arg;
use PhpParser\Node\Attribute;
use PhpParser\Node\AttributeGroup;
use PhpParser\Node\Expr;
use PhpParser\Node\Expr\Array_;
use PhpParser\Node\Expr\ArrowFunction;
use PhpParser\Node\Expr\ClassConstFetch;
use PhpParser\Node\Expr\Closure;
use PhpParser\Node\Expr\FuncCall;
use PhpParser\Node\Expr\MethodCall;
use PhpParser\Node\Expr\PropertyFetch;
use PhpParser\Node\Expr\StaticCall;
use PhpParser\Node\Expr\Variable;
use PhpParser\Node\Identifier;
use PhpParser\Node\Name;
use PhpParser\Node\Name\FullyQualified;
use PhpParser\Node\Param;
use PhpParser\Node\Scalar\String_;
use PhpParser\Node\Stmt;
use PhpParser\Node\Expr\Assign;
use PhpParser\Node\Expr\Yield_;
use PhpParser\Node\Expr\YieldFrom;
use PhpParser\Node\Stmt\Class_;
use PhpParser\Node\Stmt\ClassMethod;
use PhpParser\Node\Stmt\Expression;
use PhpParser\Node\Stmt\Namespace_;
use PhpParser\Node\Stmt\Nop;
use PhpParser\Node\Stmt\Property;
use PhpParser\Node\Stmt\PropertyProperty;
use PhpParser\Node\Stmt\Return_;
use PhpParser\Node\Stmt\TraitUse;
use PhpParser\Node\Stmt\GroupUse;
use PhpParser\Node\Stmt\Use_;
use PhpParser\NodeVisitor;
use Rector\Contract\Rector\ConfigurableRectorInterface;
use Rector\PhpParser\Node\FileNode;
use Rector\Rector\AbstractRector;
use Symplify\RuleDocGenerator\ValueObject\CodeSample\CodeSample;
use Symplify\RuleDocGenerator\ValueObject\RuleDefinition;

final class PestFileToPhpUnitClassRector extends AbstractRector implements ConfigurableRectorInterface
{
    public const INFER_NAMESPACE = 'infer_namespace';

    private const PEST_FUNCTIONS = [
        'test', 'it', 'beforeEach', 'afterEach', 'beforeAll', 'afterAll',
        'uses', 'dataset', 'describe', 'covers', 'coversNothing', 'arch',
        'expect',
    ];

    /** @var array<string, bool> Track which files we've already processed */
    private array $processedFiles = [];

    private bool $inferNamespace = false;

    /**
     * @param mixed[] $configuration
     */
    public function configure(array $configuration): void
    {
        $this->inferNamespace = (bool) ($configuration[self::INFER_NAMESPACE] ?? false);
    }

    public function getRuleDefinition(): RuleDefinition
    {
        return new RuleDefinition(
            'Convert Pest test file to PHPUnit test class',
            [
                new CodeSample(
                    <<<'CODE_SAMPLE'
test('adds numbers', function () {
    expect(1 + 1)->toBe(2);
});
CODE_SAMPLE,
                    <<<'CODE_SAMPLE'
class ExampleTest extends \PHPUnit\Framework\TestCase
{
    public function test_adds_numbers(): void
    {
        $this->assertSame(2, 1 + 1);
    }
}
CODE_SAMPLE,
                ),
            ]
        );
    }

    public function getNodeTypes(): array
    {
        return [Expression::class, Use_::class, GroupUse::class];
    }

    /**
     * @return Node|Node[]|null|NodeVisitor::REMOVE_NODE
     */
    public function refactor(Node $node): mixed
    {
        // Strip `use function Pest\...` imports
        if ($node instanceof Use_ && $node->type === Use_::TYPE_FUNCTION) {
            return $this->refactorPestFunctionUse($node);
        }
        if ($node instanceof GroupUse && $node->type === Use_::TYPE_FUNCTION) {
            if (str_starts_with($node->prefix->toString(), 'Pest\\')) {
                return NodeVisitor::REMOVE_NODE;
            }
            return null;
        }

        if (! $node instanceof Expression) {
            return null;
        }

        if (! $this->isPestExpression($node)) {
            return null;
        }

        $filePath = $this->file->getFilePath();

        // Already processed this file — remove remaining pest expressions
        if (isset($this->processedFiles[$filePath])) {
            return NodeVisitor::REMOVE_NODE;
        }

        // Mark file as processed
        $this->processedFiles[$filePath] = true;

        // Collect all statements from the file
        $allStmts = $this->file->getNewStmts();

        // Find the actual top-level stmts (handle FileNode and namespace wrappers)
        $fileNode = null;
        $namespace = null;
        $topLevelStmts = $allStmts;
        $preservedStmts = [];

        foreach ($allStmts as $stmt) {
            if ($stmt instanceof FileNode) {
                $fileNode = $stmt;
                $topLevelStmts = $stmt->stmts;

                // Check for namespace inside FileNode
                foreach ($stmt->stmts as $innerStmt) {
                    if ($innerStmt instanceof Namespace_) {
                        $namespace = $innerStmt;
                        $topLevelStmts = $innerStmt->stmts;
                        break;
                    }
                }
                break;
            }

            if ($stmt instanceof Namespace_) {
                $namespace = $stmt;
                $topLevelStmts = $stmt->stmts;
                break;
            }
        }

        // Separate pest statements from non-pest statements (declare, use imports, etc.)
        $pestStmts = [];
        foreach ($topLevelStmts as $stmt) {
            if ($stmt instanceof Expression && $this->isPestExpression($stmt)) {
                $pestStmts[] = $stmt;
            } else {
                $preservedStmts[] = $stmt;
            }
        }

        if ($pestStmts === []) {
            return null;
        }

        // Pre-scan: collect expect()->extend() definitions into the registry
        CustomExpectationRegistry::collectFromStatements($pestStmts);

        // Filter out expect()->extend() statements — they've been consumed by the registry
        $nonExtendStmts = [];
        foreach ($pestStmts as $pestStmt) {
            $expr = $pestStmt->expr;
            $chainMods = [];
            $rootCall = $this->unwrapChain($expr, $chainMods);
            if ($rootCall instanceof FuncCall) {
                $fn = $rootCall->name instanceof Name ? $rootCall->name->toString() : null;
                if ($fn === 'expect') {
                    $isExtend = false;
                    foreach ($chainMods as $mod) {
                        if ($mod['name'] === 'extend') {
                            $isExtend = true;
                            break;
                        }
                    }
                    if ($isExtend) {
                        continue;
                    }
                }
            }
            $nonExtendStmts[] = $pestStmt;
        }

        // If only extend() definitions remain, no class to generate — leave file unchanged
        if ($nonExtendStmts === []) {
            unset($this->processedFiles[$filePath]);
            return null;
        }

        // Derive class name from file path
        $className = NameHelper::fileNameToClassName(basename($filePath));

        // Process all pest statements
        $extendsClass = new FullyQualified('PHPUnit\\Framework\\TestCase');
        $traitUses = [];
        $methods = [];
        $classAttributes = [];
        $dataProviders = [];
        $inlineProviderCounter = 0;

        foreach ($nonExtendStmts as $pestStmt) {
            $expr = $pestStmt->expr;

            // Unwrap method chains to find the root function call
            $chainModifiers = [];
            $rootCall = $this->unwrapChain($expr, $chainModifiers);

            if (! $rootCall instanceof FuncCall) {
                continue;
            }

            $funcName = $rootCall->name instanceof Name ? $rootCall->name->toString() : null;

            if ($funcName === null) {
                continue;
            }

            switch ($funcName) {
                case 'test':
                case 'it':
                    $result = $this->processTestCall($rootCall, $funcName, $chainModifiers, $dataProviders, $inlineProviderCounter);
                    if ($result !== null) {
                        $methods[] = $result['method'];
                        if (isset($result['providers'])) {
                            array_push($dataProviders, ...$result['providers']);
                        }
                        $inlineProviderCounter = $result['providerCounter'];
                    }
                    break;

                case 'beforeEach':
                case 'afterEach':
                case 'beforeAll':
                case 'afterAll':
                    $method = $this->processHook($rootCall, $funcName);
                    if ($method !== null) {
                        $methods[] = $method;
                    }
                    break;

                case 'uses':
                    $this->processUses($rootCall, $extendsClass, $traitUses);

                    foreach ($chainModifiers as $modifier) {
                        switch ($modifier['name']) {
                            case 'beforeEach':
                            case 'afterEach':
                            case 'beforeAll':
                            case 'afterAll':
                                $hookCall = new FuncCall(
                                    new Name($modifier['name']),
                                    $modifier['args']
                                );
                                $hookMethod = $this->processHook($hookCall, $modifier['name']);
                                if ($hookMethod !== null) {
                                    $methods[] = $hookMethod;
                                }
                                break;

                            case 'group':
                                foreach ($modifier['args'] as $groupArg) {
                                    if ($groupArg->value instanceof String_) {
                                        $classAttributes[] = new AttributeGroup([
                                            new Attribute(
                                                new FullyQualified('PHPUnit\\Framework\\Attributes\\Group'),
                                                [new Arg($groupArg->value)]
                                            ),
                                        ]);
                                    }
                                }
                                break;

                            case 'in':
                                // Scopes which directories uses() applies to — not relevant for single-file conversion
                                break;
                        }
                    }
                    break;

                case 'dataset':
                    $provider = $this->processDataset($rootCall);
                    if ($provider !== null) {
                        $dataProviders[] = $provider;
                    }
                    break;

                case 'describe':
                    $describeMethods = $this->processDescribe($rootCall, '', $dataProviders, $inlineProviderCounter, $chainModifiers);
                    foreach ($describeMethods as $dm) {
                        $methods[] = $dm['method'];
                        if (isset($dm['providers'])) {
                            array_push($dataProviders, ...$dm['providers']);
                        }
                        $inlineProviderCounter = $dm['providerCounter'];
                    }
                    break;

                case 'covers':
                    $attr = $this->processCovers($rootCall);
                    if ($attr !== null) {
                        $classAttributes[] = $attr;
                    }
                    break;

                case 'coversNothing':
                    $classAttributes[] = new AttributeGroup([
                        new Attribute(new FullyQualified('PHPUnit\\Framework\\Attributes\\CoversNothing')),
                    ]);
                    break;

                case 'arch':
                    $method = $this->processArch($rootCall);
                    if ($method !== null) {
                        $methods[] = $method;
                    }
                    break;

                case 'expect':
                    // Non-extend expect() calls at top-level — skip silently
                    break;
            }
        }

        // Collect property writes from all method bodies
        $propertyNames = [];
        foreach ($methods as $method) {
            if ($method instanceof ClassMethod && $method->stmts !== null) {
                $propertyNames = array_merge($propertyNames, $this->collectPropertyWrites($method->stmts));
            }
        }
        $propertyNames = array_unique($propertyNames);

        // Build class body
        $classStmts = [];

        if ($traitUses !== []) {
            $classStmts[] = new TraitUse($traitUses);
        }

        // Property declarations (after trait uses, before providers/methods)
        foreach ($propertyNames as $propName) {
            $classStmts[] = new Property(
                Class_::MODIFIER_PROTECTED,
                [new PropertyProperty($propName)]
            );
        }

        foreach ($dataProviders as $provider) {
            $classStmts[] = $provider;
        }

        foreach ($methods as $method) {
            $classStmts[] = $method;
        }

        $class = new Class_(
            $className,
            [
                'extends' => $extendsClass,
                'stmts' => $classStmts,
                'attrGroups' => $classAttributes,
            ]
        );

        $class->setAttribute('startLine', $node->getStartLine());
        $class->setAttribute('endLine', $node->getEndLine());

        // If there's no namespace and inference is enabled, try to derive one from composer.json PSR-4 mappings
        $inferredNamespace = false;
        if ($namespace === null && $this->inferNamespace) {
            $inferredNs = NameHelper::inferNamespaceFromPath($filePath);
            if ($inferredNs !== null) {
                // Separate declare statements (must stay before namespace) from other preserved stmts
                $beforeNs = [];
                $insideNs = [];
                foreach ($preservedStmts as $s) {
                    if ($s instanceof Stmt\Declare_) {
                        $beforeNs[] = $s;
                    } else {
                        $insideNs[] = $s;
                    }
                }
                $namespace = new Namespace_(new Name($inferredNs), [...$insideNs, $class]);
                $preservedStmts = $beforeNs;
                $inferredNamespace = true;
                $class->namespacedName = new Name($inferredNs . '\\' . $className);
            } else {
                $class->namespacedName = new Name($className);
            }
        } elseif ($namespace !== null) {
            $nsName = $namespace->name instanceof Name ? $namespace->name->toString() : '';
            $class->namespacedName = new Name($nsName !== '' ? $nsName . '\\' . $className : $className);
        } else {
            $class->namespacedName = new Name($className);
        }

        // Replace the namespace stmts or file stmts
        if ($namespace !== null && ! $inferredNamespace) {
            // Existing namespace — update its stmts in place
            $namespace->stmts = [...$preservedStmts, $class];
            $this->file->changeNewStmts($allStmts);
        } elseif ($inferredNamespace) {
            // Newly created namespace — replace all stmts
            $newStmts = [...$preservedStmts, $namespace];
            if ($fileNode !== null) {
                $fileNode->stmts = $newStmts;
                $this->file->changeNewStmts($allStmts);
            } else {
                $this->file->changeNewStmts($newStmts);
            }
        } elseif ($fileNode !== null) {
            $fileNode->stmts = [...$preservedStmts, $class];
            $this->file->changeNewStmts($allStmts);
        } else {
            $newStmts = [...$preservedStmts, $class];
            $this->file->changeNewStmts($newStmts);
        }

        // Return the replacement for this first pest expression
        if ($inferredNamespace) {
            // Return namespace wrapping the class — replaces the pest expression at top level
            return $namespace;
        }
        return $class;
    }

    /**
     * Check if an Expression node contains a Pest function call.
     */
    private function isPestExpression(Expression $node): bool
    {
        $expr = $node->expr;

        // Unwrap method chains and property fetches (e.g., it(...)->skip()->group(), it(...)->expect(...)->each->not->toBeUsed())
        while ($expr instanceof MethodCall || $expr instanceof PropertyFetch) {
            $expr = $expr->var;
        }

        if (! $expr instanceof FuncCall) {
            return false;
        }

        $funcName = $expr->name instanceof Name ? $expr->name->toString() : null;

        return $funcName !== null && in_array($funcName, self::PEST_FUNCTIONS, true);
    }

    /**
     * Unwrap a method call chain to find the root FuncCall and collect modifiers.
     *
     * @param list<array{name: string, args: list<Arg>}> $modifiers collected in reverse (outermost first)
     */
    private function unwrapChain(Expr $expr, array &$modifiers): Expr
    {
        while ($expr instanceof MethodCall || $expr instanceof PropertyFetch) {
            if ($expr instanceof MethodCall) {
                $name = $expr->name instanceof Identifier ? $expr->name->name : null;
                if ($name !== null) {
                    $args = [];
                    foreach ($expr->args as $arg) {
                        if ($arg instanceof Arg) {
                            $args[] = $arg;
                        }
                    }
                    array_unshift($modifiers, ['name' => $name, 'args' => $args]);
                }
            } else {
                // PropertyFetch (e.g., ->each, ->not)
                $name = $expr->name instanceof Identifier ? $expr->name->name : null;
                if ($name !== null) {
                    array_unshift($modifiers, ['name' => $name, 'args' => []]);
                }
            }
            $expr = $expr->var;
        }

        return $expr;
    }

    /**
     * Process a test() or it() call into a class method.
     *
     * @param list<array{name: string, args: list<Arg>}> $chainModifiers
     * @param list<ClassMethod> $dataProviders
     * @return array{method: ClassMethod, provider?: ClassMethod, providerCounter: int}|null
     */
    private function processTestCall(FuncCall $call, string $funcName, array $chainModifiers, array &$dataProviders, int $providerCounter, string $descriptionPrefix = '', array $scopedBeforeEach = [], array $scopedAfterEach = []): ?array
    {
        $args = $call->args;
        if (count($args) < 1) {
            return null;
        }

        // First arg: description
        $descriptionArg = $args[0] instanceof Arg ? $args[0]->value : null;
        $description = $descriptionArg instanceof String_ ? $descriptionArg->value : 'unnamed test';

        if ($descriptionPrefix !== '') {
            $description = $descriptionPrefix . ' ' . $description;
        }

        $methodName = NameHelper::descriptionToMethodName($description, $funcName === 'it' ? 'it' : 'test');

        // Second arg: closure or arrow function
        $closureArg = (count($args) >= 2 && $args[1] instanceof Arg) ? $args[1]->value : null;
        $closure = $closureArg instanceof Closure ? $closureArg : null;
        $arrowFn = $closureArg instanceof ArrowFunction ? $closureArg : null;

        $params = [];
        $body = [];

        if ($closure !== null) {
            $params = $closure->params;
            $body = $closure->stmts;
        } elseif ($arrowFn !== null) {
            $params = $arrowFn->params;
            $body = [new Expression($arrowFn->expr)];
        }

        // Detect higher-order arch tests: it('...')->expect(...)->each->not->toBeUsed()
        // When there's no closure and ->expect() is a chain modifier, treat as arch test
        if ($closure === null && $arrowFn === null) {
            $hasExpectModifier = false;
            foreach ($chainModifiers as $modifier) {
                if ($modifier['name'] === 'expect') {
                    $hasExpectModifier = true;
                    break;
                }
            }
            if ($hasExpectModifier) {
                return $this->processArchTestMethod($methodName, $description);
            }
        }

        // Process chain modifiers
        $methodAttributes = [];
        $prependStmts = [];
        $skipReason = null;
        $conditionalSkip = null;
        $todoReason = null;
        $providerMethods = [];
        $repeatExpr = null;
        $afterStmts = [];
        $withIndex = 0;

        foreach ($chainModifiers as $modifier) {
            switch ($modifier['name']) {
                case 'with':
                    $result = $this->processWithModifier($modifier['args'], $methodName, $providerCounter, $withIndex);
                    $withIndex++;
                    if ($result !== null) {
                        $methodAttributes[] = $result['attribute'];
                        if (isset($result['provider'])) {
                            $providerMethods[] = $result['provider'];
                            $providerCounter = $result['counter'];
                        }
                    }
                    break;

                case 'throws':
                    $throwsStmts = $this->processThrowsModifier($modifier['args']);
                    array_push($prependStmts, ...$throwsStmts);
                    break;

                case 'skip':
                    $argCount = count($modifier['args']);
                    if ($argCount === 0) {
                        $skipReason = 'Skipped';
                    } elseif ($modifier['args'][0]->value instanceof String_) {
                        $skipReason = $modifier['args'][0]->value->value;
                    } else {
                        $skipCondition = $modifier['args'][0]->value;
                        // If the condition is a closure/arrow function, invoke it
                        if ($skipCondition instanceof Closure || $skipCondition instanceof ArrowFunction) {
                            $skipCondition = new FuncCall($skipCondition);
                        }
                        $skipReasonStr = ($argCount >= 2 && $modifier['args'][1]->value instanceof String_)
                            ? $modifier['args'][1]->value->value
                            : 'Skipped';
                        $conditionalSkip = ['condition' => $skipCondition, 'reason' => $skipReasonStr];
                    }
                    break;

                case 'todo':
                    $todoReason = 'TODO';
                    break;

                case 'group':
                    foreach ($modifier['args'] as $groupArg) {
                        if ($groupArg->value instanceof String_) {
                            $methodAttributes[] = new AttributeGroup([
                                new Attribute(
                                    new FullyQualified('PHPUnit\\Framework\\Attributes\\Group'),
                                    [new Arg($groupArg->value)]
                                ),
                            ]);
                        }
                    }
                    break;

                case 'depends':
                    foreach ($modifier['args'] as $depArg) {
                        if ($depArg->value instanceof String_) {
                            $depMethodName = NameHelper::descriptionToMethodName($depArg->value->value, 'test');
                            $methodAttributes[] = new AttributeGroup([
                                new Attribute(
                                    new FullyQualified('PHPUnit\\Framework\\Attributes\\Depends'),
                                    [new Arg(new String_($depMethodName))]
                                ),
                            ]);
                        }
                    }
                    break;

                case 'covers':
                    if (count($modifier['args']) > 0) {
                        $methodAttributes[] = new AttributeGroup([
                            new Attribute(
                                new FullyQualified('PHPUnit\\Framework\\Attributes\\CoversClass'),
                                [new Arg($modifier['args'][0]->value)]
                            ),
                        ]);
                    }
                    break;

                case 'only':
                    $methodAttributes[] = new AttributeGroup([
                        new Attribute(
                            new FullyQualified('PHPUnit\\Framework\\Attributes\\Group'),
                            [new Arg(new String_('only'))]
                        ),
                    ]);
                    break;

                case 'repeat':
                    if (count($modifier['args']) > 0) {
                        $repeatExpr = $modifier['args'][0]->value;
                    }
                    break;

                case 'after':
                    if (count($modifier['args']) > 0 && $modifier['args'][0]->value instanceof Closure) {
                        $afterStmts = $modifier['args'][0]->value->stmts;
                    } elseif (count($modifier['args']) > 0 && $modifier['args'][0]->value instanceof ArrowFunction) {
                        $afterStmts = [new Expression($modifier['args'][0]->value->expr)];
                    }
                    break;
            }
        }

        // Handle skip/todo by replacing body
        if ($todoReason !== null) {
            $body = [
                new Expression(
                    new MethodCall(
                        new Variable('this'),
                        'markTestIncomplete',
                        [new Arg(new String_($todoReason))]
                    )
                ),
            ];
        } elseif ($skipReason !== null) {
            $body = [
                new Expression(
                    new MethodCall(
                        new Variable('this'),
                        'markTestSkipped',
                        [new Arg(new String_($skipReason))]
                    )
                ),
            ];
        } elseif ($conditionalSkip !== null) {
            $body = $this->transformBody($body);
            $skipStmt = new Expression(
                new MethodCall(
                    new Variable('this'),
                    'markTestSkipped',
                    [new Arg(new String_($conditionalSkip['reason']))]
                )
            );
            $ifSkip = new Stmt\If_(
                $conditionalSkip['condition'],
                ['stmts' => [$skipStmt]]
            );
            $body = [$ifSkip, ...$body];
        } else {
            // Transform expect() chains in the body
            $body = $this->transformBody($body);
        }

        // Wrap body in for loop if ->repeat(N) was used
        if ($repeatExpr !== null) {
            $loopVar = new Variable('__repeat_i');
            $body = [
                new Stmt\For_([
                    'init' => [new Assign($loopVar, new \PhpParser\Node\Scalar\Int_(0))],
                    'cond' => [new Expr\BinaryOp\Smaller($loopVar, $repeatExpr)],
                    'loop' => [new Expr\PostInc($loopVar)],
                    'stmts' => $body,
                ]),
            ];
        }

        // Inline describe-scoped beforeEach/afterEach hooks
        if ($scopedBeforeEach !== []) {
            $body = [...$scopedBeforeEach, ...$body];
        }
        if ($scopedAfterEach !== []) {
            $body = [
                new Stmt\TryCatch(
                    $body,
                    [],
                    new Stmt\Finally_($scopedAfterEach)
                ),
            ];
        }

        // Wrap in try/finally for ->after() hook
        if ($afterStmts !== []) {
            $body = [
                new Stmt\TryCatch(
                    $body,
                    [],
                    new Stmt\Finally_($afterStmts)
                ),
            ];
        }

        // Prepend throws expectations
        $body = [...$prependStmts, ...$body];

        // Build the method
        $method = new ClassMethod(
            $methodName,
            [
                'flags' => Class_::MODIFIER_PUBLIC,
                'params' => $params,
                'returnType' => new Identifier('void'),
                'stmts' => $body,
                'attrGroups' => $methodAttributes,
            ]
        );

        $result = ['method' => $method, 'providerCounter' => $providerCounter];
        if ($providerMethods !== []) {
            $result['providers'] = $providerMethods;
        }

        return $result;
    }

    /**
     * Process a hook function (beforeEach, afterEach, etc.) into a class method.
     */
    private function processHook(FuncCall $call, string $hookName): ?ClassMethod
    {
        $mapping = HookMap::getMapping($hookName);
        if ($mapping === null) {
            return null;
        }

        [$phpunitMethod, $isStatic] = $mapping;

        $closureArg = (count($call->args) >= 1 && $call->args[0] instanceof Arg) ? $call->args[0]->value : null;
        $closure = $closureArg instanceof Closure ? $closureArg : null;
        $arrowFn = $closureArg instanceof ArrowFunction ? $closureArg : null;

        if ($closure !== null) {
            $body = $closure->stmts;
        } elseif ($arrowFn !== null) {
            $body = [new Expression($arrowFn->expr)];
        } else {
            $body = [];
        }
        $body = $this->transformBody($body);

        $flags = Class_::MODIFIER_PROTECTED;
        if ($isStatic) {
            $flags |= Class_::MODIFIER_STATIC;
        }

        $stmts = [];

        // Call parent method
        $parentCall = new Expression(
            new StaticCall(
                new Name('parent'),
                $phpunitMethod
            )
        );
        $stmts[] = $parentCall;
        array_push($stmts, ...$body);

        return new ClassMethod(
            $phpunitMethod,
            [
                'flags' => $flags,
                'returnType' => new Identifier('void'),
                'stmts' => $stmts,
            ]
        );
    }

    /**
     * Process uses() call into extends class and trait uses.
     *
     * @param list<Name> $traitUses
     */
    private function processUses(FuncCall $call, FullyQualified &$extendsClass, array &$traitUses): void
    {
        foreach ($call->args as $arg) {
            if (! $arg instanceof Arg) {
                continue;
            }

            $value = $arg->value;

            // Handle Class::class references
            if ($value instanceof ClassConstFetch && $value->name instanceof Identifier && $value->name->name === 'class') {
                $className = $value->class instanceof Name ? $value->class->toString() : null;
                if ($className === null) {
                    continue;
                }

                // If the name contains "TestCase", use it as the extends class
                if (str_contains($className, 'TestCase')) {
                    $extendsClass = new FullyQualified($className);
                } else {
                    $traitUses[] = new Name($className);
                }
            } elseif ($value instanceof String_) {
                $className = $value->value;
                if (str_contains($className, 'TestCase')) {
                    $extendsClass = new FullyQualified($className);
                } else {
                    $traitUses[] = new Name($className);
                }
            }
        }
    }

    /**
     * Process a dataset() call into a static data provider method.
     */
    private function processDataset(FuncCall $call): ?ClassMethod
    {
        $args = $call->args;
        if (count($args) < 2) {
            return null;
        }

        $nameArg = $args[0] instanceof Arg ? $args[0]->value : null;
        $datasetName = $nameArg instanceof String_ ? $nameArg->value : null;
        if ($datasetName === null) {
            return null;
        }

        $dataArg = $args[1] instanceof Arg ? $args[1]->value : null;
        if ($dataArg === null) {
            return null;
        }

        $body = [];

        if ($dataArg instanceof Closure) {
            // dataset('name', function() { ... }) — use closure body
            $body = $dataArg->stmts ?? [];
        } else {
            $body = [new Return_($dataArg)];
        }

        $methodName = $this->sanitizeProviderName($datasetName);

        $returnType = ($dataArg instanceof Closure && $this->containsYield($body))
            ? 'iterable'
            : 'array';

        return new ClassMethod(
            $methodName,
            [
                'flags' => Class_::MODIFIER_PUBLIC | Class_::MODIFIER_STATIC,
                'returnType' => new Identifier($returnType),
                'stmts' => $body,
            ]
        );
    }

    /**
     * Process a describe() block by recursively extracting tests.
     *
     * @param list<ClassMethod> $dataProviders
     * @return list<array{method: ClassMethod, provider?: ClassMethod, providerCounter: int}>
     */
    private function processDescribe(FuncCall $call, string $parentPrefix, array &$dataProviders, int $providerCounter, array $describeModifiers = [], array $scopedBeforeEach = [], array $scopedAfterEach = []): array
    {
        $results = [];
        $args = $call->args;

        if (count($args) < 2) {
            return [];
        }

        $labelArg = $args[0] instanceof Arg ? $args[0]->value : null;
        $label = $labelArg instanceof String_ ? $labelArg->value : 'describe';

        $closureArg = $args[1] instanceof Arg ? $args[1]->value : null;
        $closure = $closureArg instanceof Closure ? $closureArg : null;

        if ($closure === null) {
            return [];
        }

        $prefix = $parentPrefix !== '' ? $parentPrefix . ' ' . $label : $label;

        // Collect describe-scoped beforeEach/afterEach hooks
        $localBeforeEach = [];
        $localAfterEach = [];
        $localBeforeAllFound = false;
        $localAfterAllFound = false;

        foreach ($closure->stmts as $stmt) {
            if (! $stmt instanceof Expression) {
                continue;
            }

            $expr = $stmt->expr;
            $mods = [];
            $root = $this->unwrapChain($expr, $mods);

            if (! $root instanceof FuncCall) {
                continue;
            }

            $fn = $root->name instanceof Name ? $root->name->toString() : null;

            if ($fn === 'beforeEach') {
                $closureHookArg = (count($root->args) >= 1 && $root->args[0] instanceof Arg) ? $root->args[0]->value : null;
                if ($closureHookArg instanceof Closure) {
                    $localBeforeEach = array_merge($localBeforeEach, $closureHookArg->stmts);
                } elseif ($closureHookArg instanceof ArrowFunction) {
                    $localBeforeEach[] = new Expression($closureHookArg->expr);
                }
            } elseif ($fn === 'afterEach') {
                $closureHookArg = (count($root->args) >= 1 && $root->args[0] instanceof Arg) ? $root->args[0]->value : null;
                if ($closureHookArg instanceof Closure) {
                    $localAfterEach = array_merge($localAfterEach, $closureHookArg->stmts);
                } elseif ($closureHookArg instanceof ArrowFunction) {
                    $localAfterEach[] = new Expression($closureHookArg->expr);
                }
            } elseif ($fn === 'beforeAll') {
                $localBeforeAllFound = true;
            } elseif ($fn === 'afterAll') {
                $localAfterAllFound = true;
            }
        }

        // Merge with inherited scoped hooks (outer first, then inner)
        $mergedBeforeEach = array_merge($scopedBeforeEach, $localBeforeEach);
        $mergedAfterEach = array_merge($scopedAfterEach, $localAfterEach);

        $scopedTodos = [];
        if ($localBeforeAllFound) {
            $nop = new Nop();
            $nop->setAttribute('comments', [new Comment('// TODO(Pest): beforeAll() inside describe() cannot be scoped in PHPUnit — move to setUpBeforeClass() or inline')]);
            $scopedTodos[] = $nop;
        }
        if ($localAfterAllFound) {
            $nop = new Nop();
            $nop->setAttribute('comments', [new Comment('// TODO(Pest): afterAll() inside describe() cannot be scoped in PHPUnit — move to tearDownAfterClass() or inline')]);
            $scopedTodos[] = $nop;
        }

        foreach ($closure->stmts as $stmt) {
            if (! $stmt instanceof Expression) {
                continue;
            }

            $expr = $stmt->expr;
            $chainModifiers = [];
            $rootCall = $this->unwrapChain($expr, $chainModifiers);

            if (! $rootCall instanceof FuncCall) {
                continue;
            }

            $funcName = $rootCall->name instanceof Name ? $rootCall->name->toString() : null;

            // Skip hook calls — already collected above
            if ($funcName === 'beforeEach' || $funcName === 'afterEach' || $funcName === 'beforeAll' || $funcName === 'afterAll') {
                continue;
            }

            if ($funcName === 'test' || $funcName === 'it') {
                $mergedModifiers = array_merge($describeModifiers, $chainModifiers);
                $result = $this->processTestCall($rootCall, $funcName, $mergedModifiers, $dataProviders, $providerCounter, $prefix, array_merge($scopedTodos, $mergedBeforeEach), $mergedAfterEach);
                if ($result !== null) {
                    $results[] = $result;
                    $providerCounter = $result['providerCounter'];
                }
            } elseif ($funcName === 'describe') {
                $nestedModifiers = array_merge($describeModifiers, $chainModifiers);
                $nestedResults = $this->processDescribe($rootCall, $prefix, $dataProviders, $providerCounter, $nestedModifiers, array_merge($scopedTodos, $mergedBeforeEach), $mergedAfterEach);
                foreach ($nestedResults as $nr) {
                    $results[] = $nr;
                    $providerCounter = $nr['providerCounter'];
                }
            }
        }

        return $results;
    }

    /**
     * Process covers() call into a PHPUnit attribute.
     */
    private function processCovers(FuncCall $call): ?AttributeGroup
    {
        if (count($call->args) < 1 || ! $call->args[0] instanceof Arg) {
            return null;
        }

        return new AttributeGroup([
            new Attribute(
                new FullyQualified('PHPUnit\\Framework\\Attributes\\CoversClass'),
                [new Arg($call->args[0]->value)]
            ),
        ]);
    }

    /**
     * Process arch() call into a skipped test with a comment.
     */
    private function processArch(FuncCall $call): ?ClassMethod
    {
        $descriptionArg = (count($call->args) >= 1 && $call->args[0] instanceof Arg) ? $call->args[0]->value : null;
        $description = $descriptionArg instanceof String_ ? $descriptionArg->value : 'arch test';

        $methodName = NameHelper::descriptionToMethodName($description, 'test');

        $skipStmt = new Expression(
            new MethodCall(
                new Variable('this'),
                'markTestSkipped',
                [new Arg(new String_('Arch test not supported in PHPUnit: ' . $description))]
            )
        );
        $skipStmt->setDocComment(new Doc("/** Pest arch() test - manual review needed */"));

        return new ClassMethod(
            $methodName,
            [
                'flags' => Class_::MODIFIER_PUBLIC,
                'returnType' => new Identifier('void'),
                'stmts' => [$skipStmt],
            ]
        );
    }

    /**
     * Build a skipped arch test method result for higher-order arch patterns like it('...')->expect(...)->each->not->toBeUsed().
     *
     * @return array{method: ClassMethod, providerCounter: int}
     */
    private function processArchTestMethod(string $methodName, string $description): array
    {
        $skipStmt = new Expression(
            new MethodCall(
                new Variable('this'),
                'markTestSkipped',
                [new Arg(new String_('Arch test not supported in PHPUnit: ' . $description))]
            )
        );
        $skipStmt->setDocComment(new Doc("/** Pest arch() test - manual review needed */"));

        $method = new ClassMethod(
            $methodName,
            [
                'flags' => Class_::MODIFIER_PUBLIC,
                'returnType' => new Identifier('void'),
                'stmts' => [$skipStmt],
            ]
        );

        return ['method' => $method, 'providerCounter' => 0];
    }

    /**
     * Process ->with() modifier to add a data provider attribute.
     *
     * @param list<Arg> $args
     * @return array{attribute: AttributeGroup, provider?: ClassMethod, counter: int}|null
     */
    private function processWithModifier(array $args, string $testMethodName, int $counter, int $withIndex = 0): ?array
    {
        if (count($args) < 1) {
            return null;
        }

        $firstArg = $args[0]->value;

        // ->with('datasetName') — reference to a named dataset
        if ($firstArg instanceof String_) {
            $providerName = $this->sanitizeProviderName($firstArg->value);

            $attribute = new AttributeGroup([
                new Attribute(
                    new FullyQualified('PHPUnit\\Framework\\Attributes\\DataProvider'),
                    [new Arg(new String_($providerName))]
                ),
            ]);

            return ['attribute' => $attribute, 'counter' => $counter];
        }

        // ->with([...]) — inline data
        if ($firstArg instanceof Array_ || $firstArg instanceof Closure) {
            $suffix = $withIndex > 0 ? '_provider_' . ($withIndex + 1) : '_provider';
            // Strip test_ prefix to avoid PHPUnit treating provider as a test method
            $baseName = str_starts_with($testMethodName, 'test_') ? substr($testMethodName, 5) : $testMethodName;
            $providerName = $baseName . $suffix;
            $counter++;

            $body = [];
            if ($firstArg instanceof Closure) {
                $body = $firstArg->stmts ?? [];
            } else {
                // Check if items need wrapping for PHPUnit data providers
                // Pest wraps scalar values: is_array($v) ? $v : [$v]
                $needsWrapping = true;
                foreach ($firstArg->items as $item) {
                    if ($item !== null && $item->value instanceof Array_) {
                        $needsWrapping = false;
                        break;
                    }
                }

                if ($needsWrapping && $firstArg->items !== []) {
                    $wrappedItems = [];
                    foreach ($firstArg->items as $item) {
                        if ($item !== null) {
                            $wrappedItems[] = new \PhpParser\Node\ArrayItem(
                                new Array_(
                                    [new \PhpParser\Node\ArrayItem($item->value)],
                                    ['kind' => Array_::KIND_SHORT]
                                ),
                                $item->key
                            );
                        }
                    }
                    $firstArg = new Array_($wrappedItems, ['kind' => Array_::KIND_SHORT]);
                }

                $body = [new Return_($firstArg)];
            }

            $returnType = ($firstArg instanceof Closure && $this->containsYield($body))
                ? 'iterable'
                : 'array';

            $provider = new ClassMethod(
                $providerName,
                [
                    'flags' => Class_::MODIFIER_PUBLIC | Class_::MODIFIER_STATIC,
                    'returnType' => new Identifier($returnType),
                    'stmts' => $body,
                ]
            );

            $attribute = new AttributeGroup([
                new Attribute(
                    new FullyQualified('PHPUnit\\Framework\\Attributes\\DataProvider'),
                    [new Arg(new String_($providerName))]
                ),
            ]);

            return ['attribute' => $attribute, 'provider' => $provider, 'counter' => $counter];
        }

        return null;
    }

    /**
     * Process ->throws() modifier into expectException statements.
     *
     * @param list<Arg> $args
     * @return list<Stmt>
     */
    private function processThrowsModifier(array $args): array
    {
        $stmts = [];

        if (count($args) >= 1) {
            $stmts[] = new Expression(
                new MethodCall(
                    new Variable('this'),
                    'expectException',
                    [$args[0]]
                )
            );
        }

        if (count($args) >= 2) {
            $stmts[] = new Expression(
                new MethodCall(
                    new Variable('this'),
                    'expectExceptionMessage',
                    [$args[1]]
                )
            );
        }

        return $stmts;
    }

    /**
     * Transform statement bodies by converting expect() chains to PHPUnit assertions
     * and replacing $this references from Pest closures.
     *
     * @param list<Stmt> $stmts
     * @return list<Stmt>
     */
    private function transformBody(array $stmts): array
    {
        $result = [];

        foreach ($stmts as $stmt) {
            if ($stmt instanceof Expression) {
                $unwound = ExpectChainUnwinder::unwind($stmt->expr);
                if ($unwound !== null) {
                    array_push($result, ...$unwound);
                    continue;
                }
            }

            // Recurse into child statement blocks (if, for, foreach, etc.)
            $this->transformChildBlocks($stmt);

            $result[] = $stmt;
        }

        // Rewrite fake() calls to \Faker\Factory::create()
        $result = array_map(fn (Stmt $s) => $this->transformFakeCallsInStmt($s), $result);

        return $result;
    }

    /**
     * Recursively transform expect() chains inside nested statement blocks.
     */
    private function transformChildBlocks(Stmt $stmt): void
    {
        if ($stmt instanceof Stmt\If_) {
            $stmt->stmts = $this->transformBody($stmt->stmts);
            foreach ($stmt->elseifs as $elseif) {
                $elseif->stmts = $this->transformBody($elseif->stmts);
            }
            if ($stmt->else !== null) {
                $stmt->else->stmts = $this->transformBody($stmt->else->stmts);
            }
        } elseif ($stmt instanceof Stmt\For_ || $stmt instanceof Stmt\Foreach_ || $stmt instanceof Stmt\While_ || $stmt instanceof Stmt\Do_) {
            $stmt->stmts = $this->transformBody($stmt->stmts);
        } elseif ($stmt instanceof Stmt\TryCatch) {
            $stmt->stmts = $this->transformBody($stmt->stmts);
            foreach ($stmt->catches as $catch) {
                $catch->stmts = $this->transformBody($catch->stmts);
            }
            if ($stmt->finally !== null) {
                $stmt->finally->stmts = $this->transformBody($stmt->finally->stmts);
            }
        } elseif ($stmt instanceof Stmt\Switch_) {
            foreach ($stmt->cases as $case) {
                $case->stmts = $this->transformBody($case->stmts);
            }
        }
    }

    /**
     * Sanitize a dataset name into a valid PHP method name.
     */
    private function sanitizeProviderName(string $name): string
    {
        $sanitized = preg_replace('/[^a-zA-Z0-9_]/', '_', $name);
        $sanitized = preg_replace('/_+/', '_', $sanitized);
        $sanitized = trim($sanitized, '_');

        return $sanitized !== '' ? $sanitized : 'dataset';
    }

    /**
     * Collect all property names written via $this->prop = ... in statements.
     * Does not recurse into nested Closures or ArrowFunctions.
     *
     * @param list<Stmt> $stmts
     * @return list<string>
     */
    private function collectPropertyWrites(array $stmts): array
    {
        $names = [];

        foreach ($stmts as $stmt) {
            if ($stmt instanceof Expression && $stmt->expr instanceof Assign) {
                $var = $stmt->expr->var;
                if ($var instanceof PropertyFetch
                    && $var->var instanceof Variable
                    && $var->var->name === 'this'
                    && $var->name instanceof Identifier
                ) {
                    $names[] = $var->name->name;
                }
            }

            // Recurse into child statements (if, for, foreach, etc.) but NOT closures
            foreach ($this->getChildStmts($stmt) as $childStmts) {
                $names = array_merge($names, $this->collectPropertyWrites($childStmts));
            }
        }

        return $names;
    }

    /**
     * Get child statement blocks from a statement, excluding Closures/ArrowFunctions.
     *
     * @return list<list<Stmt>>
     */
    private function getChildStmts(Stmt $stmt): array
    {
        $blocks = [];

        if ($stmt instanceof Stmt\If_) {
            $blocks[] = $stmt->stmts;
            foreach ($stmt->elseifs as $elseif) {
                $blocks[] = $elseif->stmts;
            }
            if ($stmt->else !== null) {
                $blocks[] = $stmt->else->stmts;
            }
        } elseif ($stmt instanceof Stmt\For_ || $stmt instanceof Stmt\Foreach_ || $stmt instanceof Stmt\While_ || $stmt instanceof Stmt\Do_) {
            $blocks[] = $stmt->stmts;
        } elseif ($stmt instanceof Stmt\TryCatch) {
            $blocks[] = $stmt->stmts;
            foreach ($stmt->catches as $catch) {
                $blocks[] = $catch->stmts;
            }
            if ($stmt->finally !== null) {
                $blocks[] = $stmt->finally->stmts;
            }
        } elseif ($stmt instanceof Stmt\Switch_) {
            foreach ($stmt->cases as $case) {
                $blocks[] = $case->stmts;
            }
        }

        return $blocks;
    }

    /**
     * Check if any statement contains a Yield_ or YieldFrom expression.
     * Does not recurse into nested Closures or ArrowFunctions.
     *
     * @param list<Stmt> $stmts
     */
    private function containsYield(array $stmts): bool
    {
        foreach ($stmts as $stmt) {
            if ($this->stmtContainsYield($stmt)) {
                return true;
            }
        }

        return false;
    }

    /**
     * @param Node $node
     */
    private function stmtContainsYield(Node $node): bool
    {
        if ($node instanceof Yield_ || $node instanceof YieldFrom) {
            return true;
        }

        // Don't recurse into closures/arrow functions
        if ($node instanceof Closure || $node instanceof ArrowFunction) {
            return false;
        }

        foreach ($node->getSubNodeNames() as $name) {
            $subNode = $node->$name;
            if ($subNode instanceof Node) {
                if ($this->stmtContainsYield($subNode)) {
                    return true;
                }
            } elseif (is_array($subNode)) {
                foreach ($subNode as $item) {
                    if ($item instanceof Node && $this->stmtContainsYield($item)) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    /**
     * Handle `use function Pest\...` imports — remove Pest entries, keep others.
     *
     * @return Node|NodeVisitor::REMOVE_NODE|null
     */
    private function refactorPestFunctionUse(Use_ $node): mixed
    {
        $filtered = [];
        foreach ($node->uses as $use) {
            if (! str_starts_with($use->name->toString(), 'Pest\\')) {
                $filtered[] = $use;
            }
        }

        if ($filtered === []) {
            return NodeVisitor::REMOVE_NODE;
        }

        if (count($filtered) < count($node->uses)) {
            $node->uses = $filtered;
            return $node;
        }

        return null;
    }

    /**
     * Recursively rewrite fake() calls to \Faker\Factory::create() in a statement.
     */
    private function transformFakeCallsInStmt(Stmt $stmt): Stmt
    {
        $this->transformFakeCallsInNode($stmt);

        return $stmt;
    }

    /**
     * Recursively find and rewrite fake() FuncCall nodes in-place.
     */
    private function transformFakeCallsInNode(Node $node): void
    {
        foreach ($node->getSubNodeNames() as $name) {
            $subNode = $node->$name;

            if ($subNode instanceof Node) {
                if ($subNode instanceof FuncCall && $this->isFakeCall($subNode)) {
                    $node->$name = $this->buildFakerFactoryCreate($subNode);
                } else {
                    $this->transformFakeCallsInNode($subNode);
                }
            } elseif (is_array($subNode)) {
                foreach ($subNode as $key => $item) {
                    if ($item instanceof Node) {
                        if ($item instanceof FuncCall && $this->isFakeCall($item)) {
                            $node->$name[$key] = $this->buildFakerFactoryCreate($item);
                        } else {
                            $this->transformFakeCallsInNode($item);
                        }
                    }
                }
            }
        }
    }

    /**
     * Check if a FuncCall is a fake() call (bare or namespaced Pest\Faker\fake).
     */
    private function isFakeCall(FuncCall $call): bool
    {
        if (! $call->name instanceof Name) {
            return false;
        }

        $name = $call->name->toString();

        return $name === 'fake' || $name === 'Pest\\Faker\\fake';
    }

    /**
     * Build a \Faker\Factory::create(...) static call to replace fake(...).
     */
    private function buildFakerFactoryCreate(FuncCall $fakeCall): StaticCall
    {
        return new StaticCall(
            new FullyQualified('Faker\\Factory'),
            'create',
            $fakeCall->args
        );
    }
}
