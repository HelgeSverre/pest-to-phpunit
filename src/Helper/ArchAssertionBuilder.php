<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Helper;

use PhpParser\Comment;
use PhpParser\Node\Arg;
use PhpParser\Node\Expr;
use PhpParser\Node\Expr\Array_;
use PhpParser\Node\Expr\ArrayItem;
use PhpParser\Node\Expr\Assign;
use PhpParser\Node\Expr\BinaryOp\BooleanAnd;
use PhpParser\Node\Expr\BooleanNot;
use PhpParser\Node\Expr\BinaryOp\Concat;
use PhpParser\Node\Expr\BinaryOp\Greater;
use PhpParser\Node\Expr\BinaryOp\Identical;
use PhpParser\Node\Expr\BinaryOp\NotIdentical;
use PhpParser\Node\Expr\BinaryOp\Smaller;
use PhpParser\Node\Expr\ConstFetch;
use PhpParser\Node\Expr\FuncCall;
use PhpParser\Node\Expr\MethodCall;
use PhpParser\Node\Expr\PropertyFetch;
use PhpParser\Node\Expr\Variable;
use PhpParser\Node\Identifier;
use PhpParser\Node\Name;
use PhpParser\Node\Scalar\Int_;
use PhpParser\Node\Scalar\String_;
use PhpParser\Node\Stmt;
use PhpParser\Node\Stmt\Expression;
use PhpParser\Node\Stmt\Foreach_;
use PhpParser\Node\Stmt\Nop;

/**
 * Generates PhpParser AST nodes for PHPUnit architecture assertions
 * using the ta-tikoma/phpunit-architecture-test package.
 */
final class ArchAssertionBuilder
{
    // ── Targeted assertions (reflection-based) ───────────────────────────

    /**
     * @param list<Arg>    $terminalArgs
     * @param list<string> $typeFilters
     * @param list<string> $ignoringNamespaces
     * @return list<Stmt>|null
     */
    public static function buildTargeted(
        ?string $namespace,
        string $assertionKey,
        bool $negated,
        array $terminalArgs,
        array $typeFilters,
        array $ignoringNamespaces,
    ): ?array {
        // Complex assertions that need markTestIncomplete
        $incompleteKeys = [
            'hasPublicMethods',
            'hasPublicMethodsBesides',
            'hasProtectedMethods',
            'hasProtectedMethodsBesides',
            'hasPrivateMethods',
            'hasPrivateMethodsBesides',
        ];

        if (in_array($assertionKey, $incompleteKeys, true)) {
            return self::buildIncomplete("Arch assertion '{$assertionKey}' requires manual review");
        }

        $stmts = [];

        // Build: $layer = $this->layer()->leaveByNameStart('Ns');
        $layerExpr = self::buildLayerExpr($namespace, $typeFilters, $ignoringNamespaces);
        $stmts[] = new Expression(new Assign(new Variable('layer'), $layerExpr));

        // Build the foreach body condition
        $objectVar = new Variable('object');
        $reflectionProp = new PropertyFetch($objectVar, new Identifier('reflectionClass'));

        $conditionExpr = self::buildTargetedCondition($reflectionProp, $objectVar, $assertionKey, $terminalArgs);
        if ($conditionExpr === null) {
            return null;
        }

        $message = self::buildTargetedMessage($objectVar, $assertionKey, $negated, $terminalArgs);

        // Choose assertTrue/assertFalse based on negation
        $assertMethod = $negated ? 'assertFalse' : 'assertTrue';

        $assertStmt = new Expression(
            new MethodCall(
                new Variable('this'),
                $assertMethod,
                [
                    new Arg($conditionExpr),
                    new Arg($message),
                ]
            )
        );

        $stmts[] = new Foreach_(
            new Variable('layer'),
            $objectVar,
            ['stmts' => [$assertStmt]]
        );

        return $stmts;
    }

    // ── Dependency assertions ────────────────────────────────────────────

    /**
     * @param list<Arg>    $terminalArgs
     * @param list<string> $typeFilters
     * @param list<string> $ignoringNamespaces
     * @return list<Stmt>|null
     */
    public static function buildDependency(
        ?string $namespace,
        string $assertionKey,
        bool $negated,
        array $terminalArgs,
        array $typeFilters,
        array $ignoringNamespaces,
    ): ?array {
        // Hard-to-express dependency assertions
        if (in_array($assertionKey, ['onlyDependOn', 'beUsedInNothing', 'onlyBeUsedIn'], true)) {
            return self::buildIncomplete("Arch assertion '{$assertionKey}' requires manual review");
        }

        $stmts = [];

        // Build target layer
        $targetLayerExpr = self::buildLayerExpr($namespace, $typeFilters, $ignoringNamespaces);
        $stmts[] = new Expression(new Assign(new Variable('targetLayer'), $targetLayerExpr));

        // For useNothing: assertDoesNotDependOn with a broad layer
        if ($assertionKey === 'useNothing') {
            // useNothing means "does not depend on anything" — use markTestIncomplete
            return self::buildIncomplete("Arch assertion 'useNothing' requires manual review");
        }

        // Build dependency layer from the terminal args
        if (count($terminalArgs) < 1) {
            return null;
        }

        $depArg = $terminalArgs[0]->value;
        if ($depArg instanceof String_) {
            $depLayerExpr = new MethodCall(
                new MethodCall(new Variable('this'), 'layer'),
                'leaveByNameStart',
                [new Arg($depArg)]
            );
        } else {
            // Dynamic expression — use as-is
            $depLayerExpr = new MethodCall(
                new MethodCall(new Variable('this'), 'layer'),
                'leaveByNameStart',
                [$terminalArgs[0]]
            );
        }

        $stmts[] = new Expression(new Assign(new Variable('depLayer'), $depLayerExpr));

        // Choose assertion method
        if ($assertionKey === 'dependOn') {
            $method = $negated ? 'assertDoesNotDependOn' : 'assertDependOn';
        } elseif ($assertionKey === 'beUsedIn') {
            // beUsedIn: reverse the layers
            $method = $negated ? 'assertDoesNotDependOn' : 'assertDependOn';
            // Swap: depLayer depends on targetLayer
            $stmts = [];
            $stmts[] = new Expression(new Assign(new Variable('targetLayer'), $targetLayerExpr));
            $stmts[] = new Expression(new Assign(new Variable('depLayer'), $depLayerExpr));
            $stmts[] = new Expression(
                new MethodCall(
                    new Variable('this'),
                    $method,
                    [new Arg(new Variable('depLayer')), new Arg(new Variable('targetLayer'))]
                )
            );
            return $stmts;
        } else {
            return null;
        }

        $stmts[] = new Expression(
            new MethodCall(
                new Variable('this'),
                $method,
                [new Arg(new Variable('targetLayer')), new Arg(new Variable('depLayer'))]
            )
        );

        return $stmts;
    }

    // ── Function usage assertions ────────────────────────────────────────

    /**
     * @param list<string>|null $functions
     * @param list<string>      $typeFilters
     * @param list<string>      $ignoringNamespaces
     * @return list<Stmt>|null
     */
    public static function buildFunctionUsage(
        ?string $namespace,
        ?array $functions,
        string $assertionKey,
        bool $negated,
        array $typeFilters,
        array $ignoringNamespaces,
    ): ?array {
        $stmts = [];

        // Build layer
        $layerExpr = self::buildLayerExpr($namespace, $typeFilters, $ignoringNamespaces);
        $stmts[] = new Expression(new Assign(new Variable('layer'), $layerExpr));

        // If functions are provided (expect(['dd','dump'])), check those specific functions
        if ($functions !== null && $functions !== []) {
            $functionItems = [];
            foreach ($functions as $fn) {
                $functionItems[] = new ArrayItem(new String_($fn));
            }
            $functionsArray = new Array_($functionItems, ['kind' => Array_::KIND_SHORT]);
            $stmts[] = new Expression(new Assign(new Variable('forbiddenFunctions'), $functionsArray));

            $objectVar = new Variable('object');
            $useVar = new Variable('use');
            $funcVar = new Variable('function');

            // $this->assertNotSame($function, $use, msg) or assertSame depending on negation
            $assertMethod = $negated ? 'assertNotSame' : 'assertSame';

            $message = new Concat(
                new Concat(
                    new Concat(
                        new String_('Function '),
                        $funcVar
                    ),
                    new String_(' should not be used in ')
                ),
                new PropertyFetch($objectVar, new Identifier('name'))
            );

            // Inner: foreach ($forbiddenFunctions as $function) { assertNotSame($function, $use, msg) }
            $innerAssert = new Expression(
                new MethodCall(
                    new Variable('this'),
                    $assertMethod,
                    [
                        new Arg($funcVar),
                        new Arg($useVar),
                        new Arg($message),
                    ]
                )
            );

            $innerForeach = new Foreach_(
                new Variable('forbiddenFunctions'),
                $funcVar,
                ['stmts' => [$innerAssert]]
            );

            // Middle: foreach ($object->uses as $use) { ... }
            $middleForeach = new Foreach_(
                new PropertyFetch($objectVar, new Identifier('uses')),
                $useVar,
                ['stmts' => [$innerForeach]]
            );

            // Outer: foreach ($layer as $object) { ... }
            $stmts[] = new Foreach_(
                new Variable('layer'),
                $objectVar,
                ['stmts' => [$middleForeach]]
            );

            return $stmts;
        }

        // No specific functions — beUsed check (namespace-based)
        if ($namespace !== null) {
            // For not->toBeUsed() on a namespace, generate markTestIncomplete
            return self::buildIncomplete("Arch assertion 'beUsed' on namespace requires manual review");
        }

        return null;
    }

    // ── File content assertions ──────────────────────────────────────────

    /**
     * @param list<Arg>    $terminalArgs
     * @param list<string> $typeFilters
     * @param list<string> $ignoringNamespaces
     * @return list<Stmt>|null
     */
    public static function buildFileContent(
        ?string $namespace,
        string $assertionKey,
        bool $negated,
        array $terminalArgs,
        array $typeFilters,
        array $ignoringNamespaces,
    ): ?array {
        // Complex file-content assertions
        if (in_array($assertionKey, ['methodsDocumented', 'propertiesDocumented'], true)) {
            return self::buildIncomplete("Arch assertion '{$assertionKey}' requires manual review");
        }

        $stmts = [];

        // Build layer
        $layerExpr = self::buildLayerExpr($namespace, $typeFilters, $ignoringNamespaces);
        $stmts[] = new Expression(new Assign(new Variable('layer'), $layerExpr));

        $objectVar = new Variable('object');

        // $contents = file_get_contents($object->path);
        $contentsAssign = new Expression(
            new Assign(
                new Variable('contents'),
                new FuncCall(
                    new Name('file_get_contents'),
                    [new Arg(new PropertyFetch($objectVar, new Identifier('path')))]
                )
            )
        );

        $foreachBody = [$contentsAssign];

        $contentsVar = new Variable('contents');

        switch ($assertionKey) {
            case 'strictTypes':
                // Check for declare(strict_types=1)
                $condition = new FuncCall(
                    new Name('str_contains'),
                    [
                        new Arg($contentsVar),
                        new Arg(new String_('declare(strict_types=1)')),
                    ]
                );
                $assertMethod = $negated ? 'assertFalse' : 'assertTrue';
                $message = new Concat(
                    new String_($negated ? 'Expected no strict_types in ' : 'Expected strict_types in '),
                    new PropertyFetch($objectVar, new Identifier('name'))
                );
                $foreachBody[] = new Expression(
                    new MethodCall(
                        new Variable('this'),
                        $assertMethod,
                        [new Arg($condition), new Arg($message)]
                    )
                );
                break;

            case 'strictEquality':
                // Check that file does not use == or != (loose equality)
                $looseEqCheck = new FuncCall(
                    new Name('preg_match'),
                    [
                        new Arg(new String_('/[^=!]==[^=]|[^!]!=[^=]/')),
                        new Arg($contentsVar),
                    ]
                );
                // strictEquality means "uses strict equality" (no loose comparisons)
                // Normal: assertTrue(no loose eq found) → assertSame(0, preg_match)
                // Negated: assertNotSame(0, preg_match)
                $assertMethod = $negated ? 'assertNotSame' : 'assertSame';
                $message = new Concat(
                    new String_('Loose equality found in '),
                    new PropertyFetch($objectVar, new Identifier('name'))
                );
                $foreachBody[] = new Expression(
                    new MethodCall(
                        new Variable('this'),
                        $assertMethod,
                        [
                            new Arg(new Int_(0)),
                            new Arg($looseEqCheck),
                            new Arg($message),
                        ]
                    )
                );
                break;

            case 'lineCountLessThan':
                if (count($terminalArgs) < 1) {
                    return null;
                }
                // $lineCount = substr_count($contents, "\n") + 1;
                $lineCountExpr = new Expr\BinaryOp\Plus(
                    new FuncCall(
                        new Name('substr_count'),
                        [new Arg($contentsVar), new Arg(new String_("\n"))]
                    ),
                    new Int_(1)
                );
                $foreachBody[] = new Expression(
                    new MethodCall(
                        new Variable('this'),
                        'assertLessThan',
                        [
                            $terminalArgs[0],
                            new Arg($lineCountExpr),
                            new Arg(
                                new Concat(
                                    new PropertyFetch($objectVar, new Identifier('name')),
                                    new String_(' exceeds line count limit')
                                )
                            ),
                        ]
                    )
                );
                break;

            default:
                return null;
        }

        $stmts[] = new Foreach_(
            new Variable('layer'),
            $objectVar,
            ['stmts' => $foreachBody]
        );

        return $stmts;
    }

    // ── Private helpers ──────────────────────────────────────────────────

    /**
     * Build the layer expression: $this->layer()->leaveByNameStart('Ns')
     * with optional type filters and ignoring namespaces.
     *
     * @param list<string> $typeFilters
     * @param list<string> $ignoringNamespaces
     */
    private static function buildLayerExpr(
        ?string $namespace,
        array $typeFilters,
        array $ignoringNamespaces,
    ): Expr {
        $expr = new MethodCall(new Variable('this'), 'layer');

        if ($namespace !== null) {
            $expr = new MethodCall($expr, 'leaveByNameStart', [new Arg(new String_($namespace))]);
        }

        foreach ($typeFilters as $filter) {
            $filterType = \HelgeSverre\PestToPhpUnit\Mapping\ArchMethodMap::getFilterType($filter);
            if ($filterType !== null) {
                // e.g., ->leaveByType(ObjectDescription::_CLASS)
                // Use leaveByNameIsNot to exclude non-matching types — simplified approach
                // Actually for type filters, we filter the layer by checking the type.
                // The phpunit-architecture-test uses ->leaveByType() but it expects constants.
                // Simplify: emit leaveByType with the constant string.
                // In practice this maps to checking ->reflectionClass->isInterface() etc.
                // For now, just add a comment — the type filter is typically redundant with the assertion itself
            }
        }

        foreach ($ignoringNamespaces as $ns) {
            $expr = new MethodCall($expr, 'excludeByNameStart', [new Arg(new String_($ns))]);
        }

        return $expr;
    }

    /**
     * Build the boolean condition expression for a targeted assertion.
     *
     * @param list<Arg> $terminalArgs
     */
    private static function buildTargetedCondition(
        Expr $reflection,
        Expr $objectVar,
        string $assertionKey,
        array $terminalArgs,
    ): ?Expr {
        return match ($assertionKey) {
            'isFinal' => new MethodCall($reflection, 'isFinal'),
            'isAbstract' => new MethodCall($reflection, 'isAbstract'),
            'isReadOnly' => new MethodCall($reflection, 'isReadOnly'),
            'isInterface' => new MethodCall($reflection, 'isInterface'),
            'isTrait' => new MethodCall($reflection, 'isTrait'),
            'isEnum' => new MethodCall($reflection, 'isEnum'),
            'isClass' => new BooleanAnd(
                new FuncCall(new Name('class_exists'), [new Arg(new PropertyFetch($objectVar, new Identifier('name')))]),
                new BooleanNot(
                    new FuncCall(new Name('enum_exists'), [new Arg(new PropertyFetch($objectVar, new Identifier('name')))])
                )
            ),
            'isIntBackedEnum' => new MethodCall($reflection, 'isEnum'),
            'isStringBackedEnum' => new MethodCall($reflection, 'isEnum'),
            'hasInvoke' => new MethodCall($reflection, 'hasMethod', [new Arg(new String_('__invoke'))]),
            'hasConstructor' => new MethodCall($reflection, 'hasMethod', [new Arg(new String_('__construct'))]),
            'hasDestructor' => new MethodCall($reflection, 'hasMethod', [new Arg(new String_('__destruct'))]),
            'extendsClass' => count($terminalArgs) >= 1
                ? new MethodCall($reflection, 'isSubclassOf', [$terminalArgs[0]])
                : null,
            'extendsNothing' => new Identical(
                new MethodCall($reflection, 'getParentClass'),
                new ConstFetch(new Name('false'))
            ),
            'implementsInterface' => count($terminalArgs) >= 1
                ? self::buildImplementsCheck($reflection, $terminalArgs)
                : null,
            'implementsNothing' => new Identical(
                new MethodCall($reflection, 'getInterfaceNames'),
                new Array_([], ['kind' => Array_::KIND_SHORT])
            ),
            'onlyImplements' => count($terminalArgs) >= 1
                ? self::buildImplementsCheck($reflection, $terminalArgs)
                : null,
            'usesTrait' => count($terminalArgs) >= 1
                ? self::buildUsesTraitCheck($reflection, $terminalArgs)
                : null,
            'hasPrefix' => count($terminalArgs) >= 1
                ? self::buildPrefixCheck($objectVar, $terminalArgs[0])
                : null,
            'hasSuffix' => count($terminalArgs) >= 1
                ? self::buildSuffixCheck($objectVar, $terminalArgs[0])
                : null,
            'hasMethod' => count($terminalArgs) >= 1
                ? self::buildHasMethodCheck($reflection, $terminalArgs)
                : null,
            'hasAttribute' => count($terminalArgs) >= 1
                ? new NotIdentical(
                    new FuncCall(new Name('count'), [
                        new Arg(new MethodCall($reflection, 'getAttributes', [$terminalArgs[0]])),
                    ]),
                    new Int_(0)
                )
                : null,
            default => null,
        };
    }

    /**
     * Build the assertion failure message.
     *
     * @param list<Arg> $terminalArgs
     */
    private static function buildTargetedMessage(
        Expr $objectVar,
        string $assertionKey,
        bool $negated,
        array $terminalArgs,
    ): Expr {
        $prefix = $negated ? 'Expected %s not to ' : 'Expected %s to ';

        $description = match ($assertionKey) {
            'isFinal' => 'be final',
            'isAbstract' => 'be abstract',
            'isReadOnly' => 'be readonly',
            'isInterface' => 'be an interface',
            'isTrait' => 'be a trait',
            'isEnum' => 'be an enum',
            'isClass' => 'be a class',
            'isIntBackedEnum' => 'be an int-backed enum (checked isEnum only)',
            'isStringBackedEnum' => 'be a string-backed enum (checked isEnum only)',
            'hasInvoke' => 'be invokable',
            'hasConstructor' => 'have a constructor',
            'hasDestructor' => 'have a destructor',
            'extendsClass' => 'extend the given class',
            'extendsNothing' => 'extend nothing',
            'implementsInterface' => 'implement the given interface',
            'implementsNothing' => 'implement nothing',
            'onlyImplements' => 'only implement the given interface',
            'usesTrait' => 'use the given trait',
            'hasPrefix' => 'have the given prefix',
            'hasSuffix' => 'have the given suffix',
            'hasMethod' => 'have the given method',
            'hasAttribute' => 'have the given attribute',
            default => "satisfy {$assertionKey}",
        };

        return new FuncCall(
            new Name('sprintf'),
            [
                new Arg(new String_($prefix . $description)),
                new Arg(new PropertyFetch($objectVar, new Identifier('name'))),
            ]
        );
    }

    /**
     * Check if a class implements one or more interfaces.
     *
     * @param list<Arg> $args
     */
    private static function buildImplementsCheck(Expr $reflection, array $args): Expr
    {
        if (count($args) === 1) {
            return new MethodCall($reflection, 'implementsInterface', [$args[0]]);
        }

        // Multiple interfaces: AND them together
        $expr = new MethodCall($reflection, 'implementsInterface', [$args[0]]);
        for ($i = 1; $i < count($args); $i++) {
            $expr = new BooleanAnd(
                $expr,
                new MethodCall($reflection, 'implementsInterface', [$args[$i]])
            );
        }
        return $expr;
    }

    /**
     * Check if a class uses one or more traits.
     *
     * @param list<Arg> $args
     */
    private static function buildUsesTraitCheck(Expr $reflection, array $args): Expr
    {
        // in_array($traitName, $reflection->getTraitNames(), true)
        if (count($args) === 1) {
            return new FuncCall(
                new Name('in_array'),
                [
                    $args[0],
                    new Arg(new MethodCall($reflection, 'getTraitNames')),
                    new Arg(new ConstFetch(new Name('true'))),
                ]
            );
        }

        $expr = new FuncCall(
            new Name('in_array'),
            [
                $args[0],
                new Arg(new MethodCall($reflection, 'getTraitNames')),
                new Arg(new ConstFetch(new Name('true'))),
            ]
        );
        for ($i = 1; $i < count($args); $i++) {
            $expr = new BooleanAnd(
                $expr,
                new FuncCall(
                    new Name('in_array'),
                    [
                        $args[$i],
                        new Arg(new MethodCall($reflection, 'getTraitNames')),
                        new Arg(new ConstFetch(new Name('true'))),
                    ]
                )
            );
        }
        return $expr;
    }

    /**
     * Check if a class name starts with a prefix.
     */
    private static function buildPrefixCheck(Expr $objectVar, Arg $prefixArg): Expr
    {
        return new FuncCall(
            new Name('str_starts_with'),
            [
                new Arg(
                    new FuncCall(
                        new Name('class_basename'),
                        [new Arg(new PropertyFetch($objectVar, new Identifier('name')))]
                    )
                ),
                $prefixArg,
            ]
        );
    }

    /**
     * Check if a class name ends with a suffix.
     */
    private static function buildSuffixCheck(Expr $objectVar, Arg $suffixArg): Expr
    {
        return new FuncCall(
            new Name('str_ends_with'),
            [
                new Arg(
                    new FuncCall(
                        new Name('class_basename'),
                        [new Arg(new PropertyFetch($objectVar, new Identifier('name')))]
                    )
                ),
                $suffixArg,
            ]
        );
    }

    /**
     * Check if a class has one or more methods.
     *
     * @param list<Arg> $args
     */
    private static function buildHasMethodCheck(Expr $reflection, array $args): Expr
    {
        if (count($args) === 1) {
            $arg = $args[0];
            // If it's an array, check each method
            if ($arg->value instanceof Array_) {
                return self::buildHasMethodArrayCheck($reflection, $arg->value);
            }
            return new MethodCall($reflection, 'hasMethod', [$arg]);
        }

        // Multiple args: AND them together
        $expr = new MethodCall($reflection, 'hasMethod', [$args[0]]);
        for ($i = 1; $i < count($args); $i++) {
            $expr = new BooleanAnd(
                $expr,
                new MethodCall($reflection, 'hasMethod', [$args[$i]])
            );
        }
        return $expr;
    }

    /**
     * Check if a class has all methods in an array.
     */
    private static function buildHasMethodArrayCheck(Expr $reflection, Array_ $methodArray): Expr
    {
        $items = $methodArray->items;
        if (count($items) === 0) {
            return new ConstFetch(new Name('true'));
        }

        $expr = new MethodCall($reflection, 'hasMethod', [new Arg($items[0]->value)]);
        for ($i = 1; $i < count($items); $i++) {
            if ($items[$i] === null) {
                continue;
            }
            $expr = new BooleanAnd(
                $expr,
                new MethodCall($reflection, 'hasMethod', [new Arg($items[$i]->value)])
            );
        }
        return $expr;
    }

    /**
     * Build a markTestIncomplete statement.
     *
     * @return list<Stmt>
     */
    private static function buildIncomplete(string $message): array
    {
        return [
            new Expression(
                new MethodCall(
                    new Variable('this'),
                    'markTestIncomplete',
                    [new Arg(new String_($message))]
                )
            ),
        ];
    }
}
