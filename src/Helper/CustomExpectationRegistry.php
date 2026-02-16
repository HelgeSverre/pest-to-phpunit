<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Helper;

use HelgeSverre\PestToPhpUnit\Model\CustomExpectation;
use PhpParser\Node\Arg;
use PhpParser\Node\Expr\ArrowFunction;
use PhpParser\Node\Expr\Closure;
use PhpParser\Node\Expr\FuncCall;
use PhpParser\Node\Expr\MethodCall;
use PhpParser\Node\Identifier;
use PhpParser\Node\Name;
use PhpParser\Node\Scalar\String_;
use PhpParser\Node\Stmt;
use PhpParser\Node\Stmt\Expression;

final class CustomExpectationRegistry
{
    /** @var array<string, CustomExpectation> */
    private static array $expectations = [];

    public static function register(CustomExpectation $expectation): void
    {
        self::$expectations[$expectation->name] = $expectation;
    }

    public static function has(string $name): bool
    {
        return isset(self::$expectations[$name]);
    }

    public static function get(string $name): ?CustomExpectation
    {
        return self::$expectations[$name] ?? null;
    }

    /**
     * @return array<string, CustomExpectation>
     */
    public static function all(): array
    {
        return self::$expectations;
    }

    public static function clear(): void
    {
        self::$expectations = [];
    }

    /**
     * Extract expect()->extend() calls from a list of top-level statements.
     * Returns the names of any expectations that were registered.
     *
     * @param list<Stmt> $stmts
     * @return list<string>
     */
    public static function collectFromStatements(array $stmts): array
    {
        $registered = [];

        foreach ($stmts as $stmt) {
            if (! $stmt instanceof Expression) {
                continue;
            }

            $result = self::extractExtendCall($stmt->expr);
            if ($result !== null) {
                self::register($result);
                $registered[] = $result->name;
            }
        }

        return $registered;
    }

    /**
     * Try to extract a CustomExpectation from an expect()->extend('name', fn) expression.
     */
    private static function extractExtendCall(\PhpParser\Node\Expr $expr): ?CustomExpectation
    {
        // Walk method chains to find expect()->extend(...)
        if (! $expr instanceof MethodCall) {
            return null;
        }

        $name = $expr->name instanceof Identifier ? $expr->name->name : null;
        if ($name !== 'extend') {
            return null;
        }

        // The var should be expect() — a FuncCall with name 'expect'
        $var = $expr->var;
        if (! $var instanceof FuncCall) {
            return null;
        }

        $funcName = $var->name instanceof Name ? $var->name->toString() : null;
        if ($funcName !== 'expect') {
            return null;
        }

        // Args: first is the expectation name (string), second is the closure
        $args = $expr->args;
        if (count($args) < 2) {
            return null;
        }

        $nameArg = $args[0] instanceof Arg ? $args[0]->value : null;
        if (! $nameArg instanceof String_) {
            return null;
        }

        $closureArg = $args[1] instanceof Arg ? $args[1]->value : null;

        if ($closureArg instanceof Closure) {
            return new CustomExpectation(
                name: $nameArg->value,
                params: $closureArg->params,
                body: $closureArg->stmts ?? [],
                isArrow: false,
            );
        }

        if ($closureArg instanceof ArrowFunction) {
            return new CustomExpectation(
                name: $nameArg->value,
                params: $closureArg->params,
                body: [new Expression($closureArg->expr)],
                isArrow: true,
            );
        }

        return null;
    }
}
