<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Helper;

use HelgeSverre\PestToPhpUnit\Mapping\ExpectationMethodMap;
use HelgeSverre\PestToPhpUnit\Model\CustomExpectation;
use PhpParser\Comment;
use PhpParser\Node;
use PhpParser\Node\Arg;
use PhpParser\Node\Expr;
use PhpParser\Node\Expr\ClassConstFetch;
use PhpParser\Node\Expr\FuncCall;
use PhpParser\Node\Expr\Ternary;
use PhpParser\Node\Expr\MethodCall;
use PhpParser\Node\Expr\PropertyFetch;
use PhpParser\Node\Expr\Variable;
use PhpParser\Node\Identifier;
use PhpParser\Node\Name;
use PhpParser\Node\Name\FullyQualified;
use PhpParser\Node\Scalar\String_;
use PhpParser\Node\Stmt;
use PhpParser\Node\Stmt\Nop;

final class ExpectChainUnwinder
{
    private const EACH_ITEM_VAR = '__pest_each_item';

    /**
     * Given an expression that might be an expect() chain,
     * returns an array of PHPUnit assertion statement nodes,
     * or null if this is not an expect chain.
     *
     * @return list<Stmt>|null
     */
    public static function unwind(Expr $expr, bool $initialNegated = false): ?array
    {
        $segments = self::flattenChain($expr);

        if ($segments === null) {
            return null;
        }

        return self::segmentsToStatements($segments, $initialNegated);
    }

    /**
     * Flatten the chain into a list of groups, each starting with a subject.
     * Returns null if the root is not an expect() call.
     *
     * @return list<array{subject: Expr, parts: list<array{type: string, name: string, args: list<Arg>}>}>|null
     */
    private static function flattenChain(Expr $expr): ?array
    {
        $parts = [];
        $current = $expr;

        // Walk the chain from outer to inner, collecting segments
        while (true) {
            if ($current instanceof MethodCall) {
                $name = $current->name instanceof Identifier ? $current->name->name : null;
                if ($name === null) {
                    return null;
                }

                $args = [];
                foreach ($current->args as $arg) {
                    if ($arg instanceof Arg) {
                        $args[] = $arg;
                    }
                }

                $parts[] = ['type' => 'method', 'name' => $name, 'args' => $args];
                $current = $current->var;
            } elseif ($current instanceof PropertyFetch) {
                $name = $current->name instanceof Identifier ? $current->name->name : null;
                if ($name === null) {
                    return null;
                }

                $parts[] = ['type' => 'property', 'name' => $name, 'args' => []];
                $current = $current->var;
            } elseif ($current instanceof FuncCall) {
                $funcName = $current->name instanceof Name ? $current->name->toString() : null;
                if ($funcName !== 'expect') {
                    return null;
                }

                $subject = $current->args[0] ?? null;
                $subjectExpr = ($subject instanceof Arg) ? $subject->value : null;
                if ($subjectExpr === null) {
                    return null;
                }

                // Reverse parts since we walked from outer to inner
                $parts = array_reverse($parts);

                return self::splitByAnd($subjectExpr, $parts);
            } else {
                return null;
            }
        }
    }

    /**
     * Split a linear chain of parts into groups separated by and() calls.
     *
     * @param list<array{type: string, name: string, args: list<Arg>}> $parts
     * @return list<array{subject: Expr, parts: list<array{type: string, name: string, args: list<Arg>}>}>
     */
    private static function splitByAnd(Expr $initialSubject, array $parts): array
    {
        $groups = [];
        $currentSubject = $initialSubject;
        $currentParts = [];

        foreach ($parts as $part) {
            if ($part['name'] === 'and' && $part['type'] === 'method') {
                // Flush current group
                $groups[] = ['subject' => $currentSubject, 'parts' => $currentParts];
                $currentSubject = $part['args'][0]->value ?? new Variable('undefined');
                $currentParts = [];
            } else {
                $currentParts[] = $part;
            }
        }

        $groups[] = ['subject' => $currentSubject, 'parts' => $currentParts];

        return $groups;
    }

    /**
     * @param list<array{subject: Expr, parts: list<array{type: string, name: string, args: list<Arg>}>}> $groups
     * @return list<Stmt>
     */
    private static function segmentsToStatements(array $groups, bool $initialNegated = false): array
    {
        $stmts = [];

        foreach ($groups as $i => $group) {
            $groupStmts = self::processGroup($group['subject'], $group['parts'], $i === 0 ? $initialNegated : false);
            array_push($stmts, ...$groupStmts);
        }

        return $stmts;
    }

    /**
     * @param list<array{type: string, name: string, args: list<Arg>}> $parts
     * @return list<Stmt>
     */
    private static function processGroup(Expr $subject, array $parts, bool $initialNegated = false): array
    {
        $stmts = [];
        $negated = $initialNegated;
        $eachMode = false;
        $assertChaining = false;
        $currentSubject = $subject;

        foreach ($parts as $part) {
            $name = $part['name'];
            $args = $part['args'];

            if ($name === 'not') {
                $negated = true;
                continue;
            }

            if ($name === 'each') {
                if (count($args) > 0) {
                    $comment = new Nop();
                    $comment->setAttribute('comments', [new Comment('// TODO(Pest): ->each(closure) requires manual conversion to PHPUnit')]);
                    $stmts[] = $comment;
                } else {
                    $eachMode = true;
                }
                continue;
            }

            if ($name === 'json' || $name === 'defer' || $name === 'ray' || $name === 'dd' || $name === 'ddWhen' || $name === 'ddUnless') {
                continue;
            }

            if ($name === 'sequence') {
                $comment = new Nop();
                $comment->setAttribute('comments', [new Comment('// TODO(Pest): ->sequence() requires manual conversion to PHPUnit')]);
                $stmts[] = $comment;
                continue;
            }

            if ($name === 'match' || $name === 'scoped') {
                $comment = new Nop();
                $comment->setAttribute('comments', [new Comment("// TODO(Pest): ->{$name}() requires manual conversion to PHPUnit")]);
                $stmts[] = $comment;
                continue;
            }

            if ($name === 'tap') {
                if (count($args) >= 1) {
                    $tapCallable = $args[0]->value;
                    $tapParams = [];
                    $tapBody = [];

                    if ($tapCallable instanceof \PhpParser\Node\Expr\Closure) {
                        $tapParams = $tapCallable->params;
                        $tapBody = $tapCallable->stmts;
                    } elseif ($tapCallable instanceof \PhpParser\Node\Expr\ArrowFunction) {
                        $tapParams = $tapCallable->params;
                        $tapBody = [new Stmt\Expression($tapCallable->expr)];
                    }

                    if (count($tapParams) >= 1) {
                        $paramName = $tapParams[0]->var->name;
                        $stmts[] = new Stmt\Expression(
                            new \PhpParser\Node\Expr\Assign(
                                new Variable($paramName),
                                $currentSubject
                            )
                        );
                    }
                    foreach ($tapBody as $tapStmt) {
                        if ($tapStmt instanceof Stmt\Expression) {
                            $unwound = self::unwind($tapStmt->expr);
                            if ($unwound !== null) {
                                array_push($stmts, ...$unwound);
                            } else {
                                $stmts[] = $tapStmt;
                            }
                        } else {
                            $stmts[] = $tapStmt;
                        }
                    }
                }
                continue;
            }

            if ($name === 'pipe') {
                if (count($args) >= 1) {
                    $currentSubject = new FuncCall($args[0]->value, [new Arg($currentSubject)]);
                }
                continue;
            }

            if ($name === 'when' || $name === 'unless') {
                $comment = new Nop();
                $comment->setAttribute('comments', [new Comment("// TODO(Pest): ->{$name}() requires manual conversion to PHPUnit")]);
                $stmts[] = $comment;
                continue;
            }

            // toContain: use assertStringContainsString for string literal subjects, assertContains otherwise
            if ($name === 'toContain') {
                $isStringSubject = $currentSubject instanceof String_;

                if ($eachMode) {
                    // In each mode we iterate over items — can't know their type, use assertContains
                    $baseMethod = 'assertContains';
                    $negMethod = 'assertNotContains';
                } elseif ($isStringSubject) {
                    $baseMethod = 'assertStringContainsString';
                    $negMethod = 'assertStringNotContainsString';
                } else {
                    $baseMethod = 'assertContains';
                    $negMethod = 'assertNotContains';
                }

                $phpunitMethod = $negated ? $negMethod : $baseMethod;
                $negated = false;

                foreach ($args as $singleArg) {
                    if ($eachMode) {
                        $stmts[] = self::buildEachAssertion($currentSubject, $phpunitMethod, 'expected_actual', [$singleArg]);
                    } else {
                        $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, 'expected_actual', [$singleArg]);
                    }
                }
                continue;
            }

            // toMatchConstraint($constraint)
            if ($name === 'toMatchConstraint') {
                $negated = false;
                if (count($args) >= 1) {
                    $stmts[] = new Stmt\Expression(
                        new MethodCall(new Variable('this'), 'assertThat', [new Arg($currentSubject), $args[0]])
                    );
                }
                continue;
            }

            if ($name === 'toHaveLength') {
                if (count($args) >= 1) {
                    $lenExpr = new Ternary(
                        new FuncCall(new Name('is_string'), [new Arg($currentSubject)]),
                        new FuncCall(new Name('strlen'), [new Arg($currentSubject)]),
                        new FuncCall(new Name('count'), [new Arg($currentSubject)])
                    );
                    $phpunitMethod = $negated ? 'assertNotSame' : 'assertSame';
                    $negated = false;

                    if ($eachMode) {
                        $itemVar = new Variable(self::EACH_ITEM_VAR);
                        $itemLenExpr = new Ternary(
                            new FuncCall(new Name('is_string'), [new Arg($itemVar)]),
                            new FuncCall(new Name('strlen'), [new Arg($itemVar)]),
                            new FuncCall(new Name('count'), [new Arg($itemVar)])
                        );
                        $assertStmt = new Stmt\Expression(
                            new MethodCall(new Variable('this'), $phpunitMethod, [
                                $args[0],
                                new Arg($itemLenExpr),
                            ])
                        );
                        $stmts[] = new Stmt\Foreach_(
                            $currentSubject,
                            $itemVar,
                            ['stmts' => [$assertStmt]]
                        );
                    } else {
                        $stmts[] = new Stmt\Expression(
                            new MethodCall(new Variable('this'), $phpunitMethod, [
                                $args[0],
                                new Arg($lenExpr),
                            ])
                        );
                    }
                }
                continue;
            }

            // Check if this is a terminal (assertion)
            $mapping = ExpectationMethodMap::getMapping($name);
            if ($mapping !== null) {
                [$phpunitMethod, $argOrder] = $mapping;

                if ($negated) {
                    $negatedMethod = ExpectationMethodMap::getNegated($phpunitMethod);
                    if ($negatedMethod !== null) {
                        $phpunitMethod = $negatedMethod;
                    } else {
                        $comment = new Nop();
                        $comment->setAttribute('comments', [new Comment("// TODO(Pest): not->{$name}() has no direct PHPUnit equivalent")]);
                        $stmts[] = $comment;
                        $negated = false;
                        continue;
                    }
                    $negated = false;
                }

                if ($eachMode) {
                    $stmts[] = self::buildEachAssertion($currentSubject, $phpunitMethod, $argOrder, $args);
                    // Don't reset eachMode - it applies to subsequent assertions too
                } else {
                    $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, $argOrder, $args);
                }

                continue;
            }

            if ($name === 'toThrow') {
                $throwStmts = self::buildToThrow($currentSubject, $args, $negated);
                array_push($stmts, ...$throwStmts);
                $negated = false;
                continue;
            }

            if ($name === 'toMatchArray') {
                $phpunitMethod = $negated ? 'assertNotEquals' : 'assertEquals';
                $negated = false;
                $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, 'expected_actual', $args);
                continue;
            }

            if ($name === 'toHaveProperty') {
                $phpunitMethod = $negated ? 'assertObjectNotHasProperty' : 'assertObjectHasProperty';
                $negated = false;
                if (count($args) >= 1) {
                    $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, 'expected_actual', [$args[0]]);
                }
                continue;
            }

            if ($name === 'toHaveProperties') {
                $negated_local = $negated;
                $negated = false;
                if (count($args) >= 1 && $args[0]->value instanceof \PhpParser\Node\Expr\Array_) {
                    $isAssociative = false;
                    foreach ($args[0]->value->items as $item) {
                        if ($item !== null && $item->key !== null) {
                            $isAssociative = true;
                            break;
                        }
                    }

                    if ($isAssociative) {
                        foreach ($args[0]->value->items as $item) {
                            if ($item === null || $item->key === null) {
                                continue;
                            }
                            $propAccess = new PropertyFetch($currentSubject, new Identifier($item->key->value));
                            $assertMethod = $negated_local ? 'assertNotSame' : 'assertSame';
                            $stmts[] = new Stmt\Expression(
                                new MethodCall(new Variable('this'), $assertMethod, [new Arg($item->value), new Arg($propAccess)])
                            );
                        }
                    } else {
                        $phpunitMethod = $negated_local ? 'assertObjectNotHasProperty' : 'assertObjectHasProperty';
                        foreach ($args[0]->value->items as $item) {
                            if ($item !== null) {
                                $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, 'expected_actual', [new Arg($item->value)]);
                            }
                        }
                    }
                }
                continue;
            }

            if ($name === 'toHaveKeys') {
                // toHaveKeys(['a', 'b']) → multiple assertArrayHasKey calls
                $phpunitMethod = $negated ? 'assertArrayNotHasKey' : 'assertArrayHasKey';
                $negated = false;
                if (count($args) >= 1 && $args[0]->value instanceof \PhpParser\Node\Expr\Array_) {
                    foreach ($args[0]->value->items as $item) {
                        if ($item !== null) {
                            $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, 'expected_actual', [new Arg($item->value)]);
                        }
                    }
                }
                continue;
            }

            if ($name === 'toContainOnlyInstancesOf') {
                $phpunitMethod = $negated ? 'assertContainsNotOnlyInstancesOf' : 'assertContainsOnlyInstancesOf';
                $negated = false;
                $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, 'expected_actual', $args);
                continue;
            }

            if ($name === 'toBeBetween') {
                // expect($x)->toBeBetween(1, 10)
                if (count($args) >= 2) {
                    $gte = $negated ? 'assertLessThan' : 'assertGreaterThanOrEqual';
                    $lte = $negated ? 'assertGreaterThan' : 'assertLessThanOrEqual';
                    $negated = false;
                    $stmts[] = self::buildAssertion($currentSubject, $gte, 'expected_actual', [$args[0]]);
                    $stmts[] = self::buildAssertion($currentSubject, $lte, 'expected_actual', [$args[1]]);
                }
                continue;
            }

            if ($name === 'toHaveMethod' || $name === 'toHaveMethods') {
                // Skip - no direct PHPUnit equivalent, emit assertTrue with method_exists
                $phpunitMethod = $negated ? 'assertFalse' : 'assertTrue';
                $negated = false;
                if (count($args) >= 1) {
                    $methodExistsCall = new FuncCall(
                        new Name('method_exists'),
                        [new Arg($currentSubject), $args[0]]
                    );
                    $stmts[] = new Stmt\Expression(
                        new MethodCall(
                            new Variable('this'),
                            $phpunitMethod,
                            [new Arg($methodExistsCall)]
                        )
                    );
                }
                continue;
            }

            if ($name === 'toBeTruthy') {
                $phpunitMethod = $negated ? 'assertEmpty' : 'assertNotEmpty';
                $negated = false;
                $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, 'actual_only', []);
                continue;
            }

            if ($name === 'toBeFalsy') {
                $phpunitMethod = $negated ? 'assertNotEmpty' : 'assertEmpty';
                $negated = false;
                $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, 'actual_only', []);
                continue;
            }

            if ($name === 'toBeIn') {
                $phpunitMethod = $negated ? 'assertNotContains' : 'assertContains';
                $negated = false;
                if (count($args) >= 1) {
                    // Swap: assertContains($needle=$subject, $haystack=$arg)
                    $stmts[] = new Stmt\Expression(
                        new MethodCall(
                            new Variable('this'),
                            $phpunitMethod,
                            [new Arg($currentSubject), $args[0]]
                        )
                    );
                }
                continue;
            }

            if ($name === 'toMatchObject') {
                $phpunitMethod = $negated ? 'assertNotEquals' : 'assertEquals';
                $negated = false;
                $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, 'expected_actual', $args);
                continue;
            }

            // toHaveXxxCaseKeys() — foreach over array_keys, assert regex on each
            $keyCaseMap = [
                'toHaveKebabCaseKeys' => '/^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$/',
                'toHaveCamelCaseKeys' => '/^[a-z][a-zA-Z0-9]*$/',
                'toHaveStudlyCaseKeys' => '/^[A-Z][a-zA-Z0-9]*$/',
                'toHaveSnakeCaseKeys' => '/^[a-z][a-z0-9]*(?:_[a-z0-9]+)*$/',
            ];
            if (isset($keyCaseMap[$name])) {
                $regex = $keyCaseMap[$name];
                $phpunitMethod = $negated ? 'assertDoesNotMatchRegularExpression' : 'assertMatchesRegularExpression';
                $negated = false;
                $keyVar = new Variable('__key');
                $assertStmt = new Stmt\Expression(
                    new MethodCall(
                        new Variable('this'),
                        $phpunitMethod,
                        [new Arg(new \PhpParser\Node\Scalar\String_($regex)), new Arg($keyVar)]
                    )
                );
                $stmts[] = new Stmt\Foreach_(
                    new FuncCall(new Name('array_keys'), [new Arg($currentSubject)]),
                    $keyVar,
                    ['stmts' => [$assertStmt]]
                );
                continue;
            }

            if ($name === 'toBeUppercase' || $name === 'toBeLowercase') {
                $funcName_case = $name === 'toBeUppercase' ? 'strtoupper' : 'strtolower';
                $phpunitMethod = $negated ? 'assertNotSame' : 'assertSame';
                $negated = false;
                $stmts[] = new Stmt\Expression(
                    new MethodCall(new Variable('this'), $phpunitMethod, [
                        new Arg(new FuncCall(new Name($funcName_case), [new Arg($currentSubject)])),
                        new Arg($currentSubject),
                    ])
                );
                continue;
            }

            if ($name === 'toBeAlpha'
                || $name === 'toBeAlphaNumeric' || $name === 'toBeSnakeCase' || $name === 'toBeKebabCase'
                || $name === 'toBeCamelCase' || $name === 'toBeStudlyCase' || $name === 'toBeUuid'
                || $name === 'toBeUrl' || $name === 'toBeDigits') {
                $regexMap = [
                    'toBeAlpha' => '/^[a-zA-Z]+$/',
                    'toBeAlphaNumeric' => '/^[a-zA-Z0-9]+$/',
                    'toBeSnakeCase' => '/^[a-z][a-z0-9]*(?:_[a-z0-9]+)*$/',
                    'toBeKebabCase' => '/^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$/',
                    'toBeCamelCase' => '/^[a-z][a-zA-Z0-9]*$/',
                    'toBeStudlyCase' => '/^[A-Z][a-zA-Z0-9]*$/',
                    'toBeUuid' => '/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/i',
                    'toBeUrl' => '/^https?:\\/\\/.+/',
                    'toBeDigits' => '/^\\d+$/',
                ];
                $regex = $regexMap[$name];
                $phpunitMethod = $negated ? 'assertDoesNotMatchRegularExpression' : 'assertMatchesRegularExpression';
                $negated = false;
                $stmts[] = self::buildAssertion(
                    $currentSubject,
                    $phpunitMethod,
                    'expected_actual',
                    [new Arg(new \PhpParser\Node\Scalar\String_($regex))]
                );
                continue;
            }

            // Laravel-specific expectations with reasonable PHPUnit mappings
            $laravelMappings = [
                'toBeCollection' => '\\Illuminate\\Support\\Collection',
                'toBeModel' => '\\Illuminate\\Database\\Eloquent\\Model',
                'toBeEloquentCollection' => '\\Illuminate\\Database\\Eloquent\\Collection',
            ];

            if (isset($laravelMappings[$name])) {
                $phpunitMethod = $negated ? 'assertNotInstanceOf' : 'assertInstanceOf';
                $negated = false;
                $classArg = new Arg(new ClassConstFetch(new FullyQualified(ltrim($laravelMappings[$name], '\\')), new Identifier('class')));
                $stmts[] = self::buildAssertion($currentSubject, $phpunitMethod, 'expected_actual', [$classArg]);
                continue;
            }

            // assert*() methods on the subject (Laravel TestResponse, Livewire, etc.)
            // Emit as direct method call statements: $subject->assertOk(), $subject->assertSee('x')
            // Chained asserts like ->assertOk()->assertSee('x') become a single chained expression
            if ($part['type'] === 'method' && str_starts_with($name, 'assert')) {
                $currentSubject = new MethodCall($currentSubject, new Identifier($name), $args);
                $assertChaining = true;
                continue;
            }

            // Check custom expectation registry before treating as unknown
            if ($part['type'] === 'method' && CustomExpectationRegistry::has($name)) {
                $customExpect = CustomExpectationRegistry::get($name);
                $inlinedSubject = $eachMode ? new Variable(self::EACH_ITEM_VAR) : $currentSubject;

                $inlinedStmts = CustomExpectationInliner::inline($customExpect, $inlinedSubject, $args, $negated);

                if ($eachMode && $inlinedStmts !== []) {
                    $stmts[] = new Stmt\Foreach_(
                        $currentSubject,
                        new Variable(self::EACH_ITEM_VAR),
                        ['stmts' => $inlinedStmts]
                    );
                } else {
                    array_push($stmts, ...$inlinedStmts);
                }
                $negated = false;
                continue;
            }

            // Unknown terminal expectation (starts with 'to') — emit TODO instead of silently treating as accessor
            if ($part['type'] === 'method' && str_starts_with($name, 'to') && !ExpectationMethodMap::isModifier($name)) {
                $comment = new Nop();
                $comment->setAttribute('comments', [new Comment("// TODO(Pest): Unknown expectation ->{$name}() has no PHPUnit equivalent")]);
                $stmts[] = $comment;
                $negated = false;
                continue;
            }

            // If it's a property access (accessor), apply it to the subject
            if ($part['type'] === 'property') {
                $currentSubject = new PropertyFetch($currentSubject, new Identifier($name));
                continue;
            }

            // If it's an unknown method call, treat it as a property accessor via method
            // (e.g., ->count() on collections)
            if ($part['type'] === 'method' && ! ExpectationMethodMap::isModifier($name)) {
                $currentSubject = new MethodCall($currentSubject, new Identifier($name), $args);
                continue;
            }
        }

        // If assert*() methods were chained, emit the accumulated expression as a statement
        if ($assertChaining) {
            $stmts[] = new Stmt\Expression($currentSubject);
        }

        return $stmts;
    }

    /**
     * Build a PHPUnit assertion statement.
     *
     * @param list<Arg> $terminalArgs
     */
    private static function buildAssertion(Expr $subject, string $phpunitMethod, string $argOrder, array $terminalArgs): Stmt\Expression
    {
        $assertArgs = self::buildAssertArgs($subject, $argOrder, $terminalArgs);

        return new Stmt\Expression(
            new MethodCall(
                new Variable('this'),
                $phpunitMethod,
                $assertArgs
            )
        );
    }

    /**
     * @param list<Arg> $terminalArgs
     * @return list<Arg>
     */
    private static function buildAssertArgs(Expr $subject, string $argOrder, array $terminalArgs): array
    {
        if ($argOrder === 'actual_only') {
            return [new Arg($subject)];
        }

        // expected_actual: first terminal arg is expected, subject is actual
        $args = [];
        if (count($terminalArgs) > 0) {
            $args[] = $terminalArgs[0];
        }
        $args[] = new Arg($subject);

        // Pass any remaining args
        for ($i = 1; $i < count($terminalArgs); $i++) {
            $args[] = $terminalArgs[$i];
        }

        return $args;
    }

    /**
     * Build a foreach loop for ->each assertions.
     *
     * @param list<Arg> $terminalArgs
     */
    private static function buildEachAssertion(Expr $subject, string $phpunitMethod, string $argOrder, array $terminalArgs): Stmt\Foreach_
    {
        $itemVar = new Variable(self::EACH_ITEM_VAR);
        $assertArgs = self::buildAssertArgs($itemVar, $argOrder, $terminalArgs);

        $assertStmt = new Stmt\Expression(
            new MethodCall(
                new Variable('this'),
                $phpunitMethod,
                $assertArgs
            )
        );

        return new Stmt\Foreach_(
            $subject,
            $itemVar,
            [
                'stmts' => [$assertStmt],
            ]
        );
    }

    /**
     * Build toThrow() assertion.
     *
     * @param list<Arg> $args
     * @return list<Stmt>
     */
    private static function buildToThrow(Expr $callable, array $args, bool $negated): array
    {
        $stmts = [];

        if ($negated) {
            // Determine the exception type to catch
            if (count($args) >= 1 && $args[0]->value instanceof ClassConstFetch) {
                $catchType = $args[0]->value->class;
            } else {
                $catchType = new FullyQualified('Throwable');
            }

            $exceptionVar = new Variable('__exception');

            // Build: 'Expected no exception, but ' . get_class($__exception) . ' was thrown: ' . $__exception->getMessage()
            $failMessage = new Expr\BinaryOp\Concat(
                new Expr\BinaryOp\Concat(
                    new Expr\BinaryOp\Concat(
                        new String_('Expected no exception, but '),
                        new FuncCall(new Name('get_class'), [new Arg($exceptionVar)])
                    ),
                    new String_(' was thrown: ')
                ),
                new MethodCall($exceptionVar, 'getMessage')
            );

            $tryCatch = new Stmt\TryCatch(
                // try body
                [
                    new Stmt\Expression(new FuncCall($callable)),
                    new Stmt\Expression(
                        new MethodCall(new Variable('this'), 'addToAssertionCount', [new Arg(new Node\Scalar\Int_(1))])
                    ),
                ],
                // catches
                [
                    new Stmt\Catch_(
                        [$catchType],
                        $exceptionVar,
                        [
                            new Stmt\Expression(
                                new MethodCall(new Variable('this'), 'fail', [new Arg($failMessage)])
                            ),
                        ]
                    ),
                ]
            );

            $stmts[] = $tryCatch;

            return $stmts;
        }

        if (count($args) === 0) {
            $stmts[] = new Stmt\Expression(
                new MethodCall(new Variable('this'), 'expectException', [
                    new Arg(new ClassConstFetch(new FullyQualified('Throwable'), new Identifier('class'))),
                ])
            );
        }

        if (count($args) >= 1) {
            $firstArgValue = $args[0]->value;

            if ($firstArgValue instanceof String_) {
                $strVal = $firstArgValue->value;
                if ((preg_match('/(?:Exception|Error)$/', $strVal) && preg_match('/^[a-zA-Z_\\\\][a-zA-Z0-9_\\\\]*$/', $strVal)) || str_contains($strVal, '\\')) {
                    // Class-string: toThrow('RuntimeException') or toThrow('App\Exceptions\Foo')
                    if (str_contains($strVal, '\\')) {
                        $classRef = new Arg(new ClassConstFetch(new FullyQualified(ltrim($strVal, '\\')), new Identifier('class')));
                    } else {
                        $classRef = new Arg(new ClassConstFetch(new Name($strVal), new Identifier('class')));
                    }
                    $stmts[] = new Stmt\Expression(
                        new MethodCall(new Variable('this'), 'expectException', [$classRef])
                    );
                    // Handle second arg as message when first was class-string
                    if (count($args) >= 2) {
                        $stmts[] = new Stmt\Expression(
                            new MethodCall(new Variable('this'), 'expectExceptionMessage', [$args[1]])
                        );
                    }
                } else {
                    // Plain message string
                    $stmts[] = new Stmt\Expression(
                        new MethodCall(new Variable('this'), 'expectExceptionMessage', [$args[0]])
                    );
                }
            } elseif ($firstArgValue instanceof \PhpParser\Node\Expr\New_) {
                // toThrow(new Exception('msg')) — instance
                $stmts[] = new Stmt\Expression(
                    new MethodCall(new Variable('this'), 'expectException', [
                        new Arg(new ClassConstFetch($firstArgValue->class, new Identifier('class'))),
                    ])
                );
                if (count($firstArgValue->args) >= 1) {
                    $stmts[] = new Stmt\Expression(
                        new MethodCall(new Variable('this'), 'expectExceptionMessage', [$firstArgValue->args[0]])
                    );
                }
            } else {
                // toThrow(Exception::class) — class reference
                $stmts[] = new Stmt\Expression(
                    new MethodCall(new Variable('this'), 'expectException', [$args[0]])
                );
            }
        }

        // expectExceptionMessage (second arg, when first was class)
        if (count($args) >= 2 && !($args[0]->value instanceof String_) && !($args[0]->value instanceof \PhpParser\Node\Expr\New_)) {
            $stmts[] = new Stmt\Expression(
                new MethodCall(new Variable('this'), 'expectExceptionMessage', [$args[1]])
            );
        }

        // Invoke the callable
        $stmts[] = new Stmt\Expression(
            new FuncCall($callable)
        );

        return $stmts;
    }
}
