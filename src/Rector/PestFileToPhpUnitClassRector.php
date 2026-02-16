<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Rector;

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
use PhpParser\Node\Stmt\Use_;
use PhpParser\NodeVisitor;
use Rector\PhpParser\Node\FileNode;
use Rector\Rector\AbstractRector;
use Symplify\RuleDocGenerator\ValueObject\CodeSample\CodeSample;
use Symplify\RuleDocGenerator\ValueObject\RuleDefinition;

final class PestFileToPhpUnitClassRector extends AbstractRector
{
    private const PEST_FUNCTIONS = [
        'test', 'it', 'beforeEach', 'afterEach', 'beforeAll', 'afterAll',
        'uses', 'dataset', 'describe', 'covers', 'coversNothing', 'arch',
        'expect',
    ];

    /** @var array<string, bool> Track which files we've already processed */
    private array $processedFiles = [];

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
        return [Expression::class];
    }

    /**
     * @return Node|Node[]|null|NodeVisitor::REMOVE_NODE
     */
    public function refactor(Node $node): mixed
    {
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

        // Derive class name from file path
        $className = NameHelper::fileNameToClassName(basename($filePath));

        // Process all pest statements
        $extendsClass = new FullyQualified('PHPUnit\\Framework\\TestCase');
        $traitUses = [];
        $methods = [];
        $classAttributes = [];
        $dataProviders = [];
        $inlineProviderCounter = 0;
        $customExpectations = [];

        foreach ($pestStmts as $pestStmt) {
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
                        if (isset($result['provider'])) {
                            $dataProviders[] = $result['provider'];
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
                        if (isset($dm['provider'])) {
                            $dataProviders[] = $dm['provider'];
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
                    // Check for expect()->extend() pattern
                    $hasExtend = false;
                    foreach ($chainModifiers as $mod) {
                        if ($mod['name'] === 'extend') {
                            $hasExtend = true;
                            $extendName = (count($mod['args']) > 0 && $mod['args'][0]->value instanceof String_)
                                ? $mod['args'][0]->value->value
                                : 'unknown';
                            $customExpectations[] = $extendName;
                            $nop = new Nop();
                            $nop->setAttribute('comments', [new Comment("// TODO(Pest): Custom expectation '{$extendName}' defined via expect()->extend() cannot be auto-converted to PHPUnit")]);
                            $methods[] = $nop;
                            break;
                        }
                    }
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

        if ($customExpectations !== []) {
            $docLines = ['/**'];
            $docLines[] = ' * TODO(Pest): The following custom expectations were defined via expect()->extend()';
            $docLines[] = ' * and cannot be auto-converted to PHPUnit:';
            foreach ($customExpectations as $ce) {
                $docLines[] = ' *   - ' . $ce;
            }
            $docLines[] = ' */';
            $class->setDocComment(new Doc(implode("\n", $docLines)));
        }
        $class->namespacedName = new Name($className);
        $class->setAttribute('startLine', $node->getStartLine());
        $class->setAttribute('endLine', $node->getEndLine());

        // Replace the namespace stmts or file stmts
        if ($namespace !== null) {
            $namespace->stmts = [...$preservedStmts, $class];
            $this->file->changeNewStmts($allStmts);
        } elseif ($fileNode !== null) {
            $fileNode->stmts = [...$preservedStmts, $class];
            $this->file->changeNewStmts($allStmts);
        } else {
            $newStmts = [...$preservedStmts, $class];
            $this->file->changeNewStmts($newStmts);
        }

        // Return the class to replace this first pest expression
        return $class;
    }

    /**
     * Check if an Expression node contains a Pest function call.
     */
    private function isPestExpression(Expression $node): bool
    {
        $expr = $node->expr;

        // Unwrap method chains (e.g., it(...)->skip()->group())
        while ($expr instanceof MethodCall) {
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
        while ($expr instanceof MethodCall) {
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
    private function processTestCall(FuncCall $call, string $funcName, array $chainModifiers, array &$dataProviders, int $providerCounter, string $descriptionPrefix = ''): ?array
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

        // Process chain modifiers
        $methodAttributes = [];
        $prependStmts = [];
        $skipReason = null;
        $conditionalSkip = null;
        $todoReason = null;
        $providerMethod = null;
        $repeatExpr = null;

        foreach ($chainModifiers as $modifier) {
            switch ($modifier['name']) {
                case 'with':
                    $result = $this->processWithModifier($modifier['args'], $methodName, $providerCounter);
                    if ($result !== null) {
                        $methodAttributes[] = $result['attribute'];
                        if (isset($result['provider'])) {
                            $providerMethod = $result['provider'];
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
        if ($providerMethod !== null) {
            $result['provider'] = $providerMethod;
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
    private function processDescribe(FuncCall $call, string $parentPrefix, array &$dataProviders, int $providerCounter, array $describeModifiers = []): array
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

            if ($funcName === 'test' || $funcName === 'it') {
                $mergedModifiers = array_merge($describeModifiers, $chainModifiers);
                $result = $this->processTestCall($rootCall, $funcName, $mergedModifiers, $dataProviders, $providerCounter, $prefix);
                if ($result !== null) {
                    $results[] = $result;
                    $providerCounter = $result['providerCounter'];
                }
            } elseif ($funcName === 'describe') {
                $nestedModifiers = array_merge($describeModifiers, $chainModifiers);
                $nestedResults = $this->processDescribe($rootCall, $prefix, $dataProviders, $providerCounter, $nestedModifiers);
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
     * Process ->with() modifier to add a data provider attribute.
     *
     * @param list<Arg> $args
     * @return array{attribute: AttributeGroup, provider?: ClassMethod, counter: int}|null
     */
    private function processWithModifier(array $args, string $testMethodName, int $counter): ?array
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
            $providerName = $testMethodName . '_provider';
            $counter++;

            $body = [];
            if ($firstArg instanceof Closure) {
                $body = $firstArg->stmts ?? [];
            } else {
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

            $result[] = $stmt;
        }

        return $result;
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
}
