<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Helper;

use HelgeSverre\PestToPhpUnit\Model\CustomExpectation;
use PhpParser\Comment;
use PhpParser\Node;
use PhpParser\Node\Arg;
use PhpParser\Node\Expr;
use PhpParser\Node\Expr\ArrowFunction;
use PhpParser\Node\Expr\Closure;
use PhpParser\Node\Expr\FuncCall;
use PhpParser\Node\Expr\MethodCall;
use PhpParser\Node\Expr\PropertyFetch;
use PhpParser\Node\Expr\Variable;
use PhpParser\Node\Identifier;
use PhpParser\Node\Name;
use PhpParser\Node\Stmt;
use PhpParser\Node\Stmt\Expression;
use PhpParser\Node\Stmt\Nop;
use PhpParser\Node\Stmt\Return_;
use PhpParser\NodeFinder;
use PhpParser\NodeTraverser;
use PhpParser\NodeVisitorAbstract;

final class CustomExpectationInliner
{
    /**
     * Attempt to inline a custom expectation's body at the call site.
     *
     * @param list<Arg> $callArgs The arguments passed to the custom expectation at the call site
     * @param bool      $negated  Whether ->not-> was applied
     * @return list<Stmt>
     */
    public static function inline(
        CustomExpectation $expectation,
        Expr $subject,
        array $callArgs,
        bool $negated = false,
    ): array {
        $body = self::cloneStmts($expectation->body);
        $params = $expectation->params;

        // Build parameter substitution map: closure param name → call site arg expression
        $paramMap = [];
        foreach ($params as $i => $param) {
            if ($param->var instanceof Variable && is_string($param->var->name)) {
                $paramMap[$param->var->name] = isset($callArgs[$i])
                    ? $callArgs[$i]->value
                    : ($param->default ?? new Expr\ConstFetch(new Name('null')));
            }
        }

        // Classify the body to decide strategy
        $strategy = self::classifyBody($body, $paramMap);

        if ($strategy === 'delegate') {
            return self::inlineDelegatingBody($body, $subject, $paramMap, $negated);
        }

        if ($strategy === 'mixed') {
            return self::inlineMixedBody($body, $subject, $paramMap, $negated);
        }

        // 'complex' — emit TODO with best-effort inlining
        return self::inlineComplexBody($expectation, $body, $subject, $paramMap, $negated);
    }

    /**
     * Classify the closure body into a conversion strategy.
     *
     * - 'delegate': Body only contains $this->toXxx() chains (delegates to built-in expectations)
     * - 'mixed': Body has local variables/logic + expect() calls or $this->toXxx()
     * - 'complex': Body has arbitrary code that can't be statically converted
     *
     * @param list<Stmt> $body
     * @param array<string, Expr> $paramMap
     */
    private static function classifyBody(array $body, array $paramMap): string
    {
        $hasThisMethodChain = false;
        $hasExpectCall = false;
        $hasArbitraryCode = false;
        $hasLocalVars = false;

        foreach ($body as $stmt) {
            if ($stmt instanceof Return_) {
                if ($stmt->expr !== null) {
                    // return $this; — ignore
                    if ($stmt->expr instanceof Variable && $stmt->expr->name === 'this') {
                        continue;
                    }
                    // return $this->toXxx()... — treat as delegate
                    if (self::isThisExpectChain($stmt->expr)) {
                        $hasThisMethodChain = true;
                        continue;
                    }
                    // return expect(...)... — treat as expect call
                    if (self::isExpectChain($stmt->expr)) {
                        $hasExpectCall = true;
                        continue;
                    }
                }
                continue;
            }

            if ($stmt instanceof Expression) {
                // $this->toXxx()... chain
                if (self::isThisExpectChain($stmt->expr)) {
                    $hasThisMethodChain = true;
                    continue;
                }
                // expect(...)... chain
                if (self::isExpectChain($stmt->expr)) {
                    $hasExpectCall = true;
                    continue;
                }
                // Variable assignment
                if ($stmt->expr instanceof Expr\Assign) {
                    $hasLocalVars = true;
                    // Check if RHS uses arbitrary method calls on $this->value
                    if (self::hasArbitraryThisValueUsage($stmt->expr)) {
                        $hasArbitraryCode = true;
                    }
                    continue;
                }
                // Arbitrary expression (method calls on $this->value, etc.)
                $hasArbitraryCode = true;
                continue;
            }

            // Other statement types (if, foreach, etc.)
            $hasArbitraryCode = true;
        }

        if ($hasArbitraryCode) {
            // Check if it's still partially convertible
            if ($hasExpectCall || $hasThisMethodChain) {
                return 'complex';
            }
            return 'complex';
        }

        if ($hasThisMethodChain && ! $hasLocalVars && ! $hasExpectCall) {
            return 'delegate';
        }

        if ($hasExpectCall || $hasLocalVars || $hasThisMethodChain) {
            return 'mixed';
        }

        return 'delegate';
    }

    /**
     * Inline a body that purely delegates to $this->toXxx() chains.
     * e.g.: return $this->toBeGreaterThan(0)->toBeLessThan(100);
     *
     * @param list<Stmt> $body
     * @param array<string, Expr> $paramMap
     * @return list<Stmt>
     */
    private static function inlineDelegatingBody(array $body, Expr $subject, array $paramMap, bool $negated): array
    {
        // Find the $this->toXxx()... chain expression
        $chainExpr = null;
        foreach ($body as $stmt) {
            if ($stmt instanceof Return_ && $stmt->expr !== null) {
                if ($stmt->expr instanceof Variable && $stmt->expr->name === 'this') {
                    continue;
                }
                $chainExpr = $stmt->expr;
                break;
            }
            if ($stmt instanceof Expression && self::isThisExpectChain($stmt->expr)) {
                $chainExpr = $stmt->expr;
                break;
            }
        }

        if ($chainExpr === null) {
            return [];
        }

        // Convert $this->toXxx($param) chain into expect($subject)->toXxx($param) chain
        $rewritten = self::rewriteThisChainToExpect($chainExpr, $subject, $paramMap);

        // Now unwind the expect() chain using the standard unwinder
        $result = ExpectChainUnwinder::unwind($rewritten, $negated);
        return $result ?? [];
    }

    /**
     * Inline a body that mixes local variables/logic with expect() calls.
     * e.g.: $x = $this->value + 1; expect($x)->toBe(5);
     *
     * @param list<Stmt> $body
     * @param array<string, Expr> $paramMap
     * @return list<Stmt>
     */
    private static function inlineMixedBody(array $body, Expr $subject, array $paramMap, bool $negated): array
    {
        $stmts = [];

        foreach ($body as $stmt) {
            // Skip return $this;
            if ($stmt instanceof Return_) {
                if ($stmt->expr === null) {
                    continue;
                }
                if ($stmt->expr instanceof Variable && $stmt->expr->name === 'this') {
                    continue;
                }
                // return $this->toXxx() — treat as delegate
                if (self::isThisExpectChain($stmt->expr)) {
                    $rewritten = self::rewriteThisChainToExpect($stmt->expr, $subject, $paramMap);
                    $unwound = ExpectChainUnwinder::unwind($rewritten, $negated);
                    if ($unwound !== null) {
                        array_push($stmts, ...$unwound);
                    }
                    continue;
                }
                // return expect(...)...
                if (self::isExpectChain($stmt->expr)) {
                    $rewritten = self::rewriteExpectChain($stmt->expr, $subject, $paramMap);
                    $unwound = ExpectChainUnwinder::unwind($rewritten, $negated);
                    if ($unwound !== null) {
                        array_push($stmts, ...$unwound);
                    }
                    continue;
                }
                continue;
            }

            if ($stmt instanceof Expression) {
                // $this->toXxx() chain
                if (self::isThisExpectChain($stmt->expr)) {
                    $rewritten = self::rewriteThisChainToExpect($stmt->expr, $subject, $paramMap);
                    $unwound = ExpectChainUnwinder::unwind($rewritten, $negated);
                    if ($unwound !== null) {
                        array_push($stmts, ...$unwound);
                    }
                    continue;
                }

                // expect(...) chain
                if (self::isExpectChain($stmt->expr)) {
                    $rewritten = self::rewriteExpectChain($stmt->expr, $subject, $paramMap);
                    $unwound = ExpectChainUnwinder::unwind($rewritten, $negated);
                    if ($unwound !== null) {
                        array_push($stmts, ...$unwound);
                    }
                    continue;
                }

                // Regular expression — substitute params and $this->value
                $stmts[] = new Expression(self::substituteExpr($stmt->expr, $subject, $paramMap));
                continue;
            }

            // Other statements — substitute and pass through
            $stmts[] = self::substituteStmt($stmt, $subject, $paramMap);
        }

        return $stmts;
    }

    /**
     * Inline a complex body with a TODO comment and best-effort conversion.
     *
     * @param list<Stmt> $body
     * @param array<string, Expr> $paramMap
     * @return list<Stmt>
     */
    private static function inlineComplexBody(
        CustomExpectation $expectation,
        array $body,
        Expr $subject,
        array $paramMap,
        bool $negated,
    ): array {
        $stmts = [];

        $comment = new Nop();
        $comment->setAttribute('comments', [
            new Comment("// TODO(Pest): Custom expectation ->{$expectation->name}() was defined via expect()->extend() and requires manual conversion"),
        ]);
        $stmts[] = $comment;

        // Best-effort: substitute $this->value and params, convert expect() chains where possible
        $subjectVar = new Variable('__pest_subject');
        $stmts[] = new Expression(new Expr\Assign($subjectVar, $subject));

        foreach ($body as $stmt) {
            // Skip return $this;
            if ($stmt instanceof Return_) {
                if ($stmt->expr === null || ($stmt->expr instanceof Variable && $stmt->expr->name === 'this')) {
                    continue;
                }
                // Try to handle return expect(...)
                if (self::isExpectChain($stmt->expr)) {
                    $rewritten = self::rewriteExpectChain($stmt->expr, $subjectVar, $paramMap);
                    $unwound = ExpectChainUnwinder::unwind($rewritten);
                    if ($unwound !== null) {
                        array_push($stmts, ...$unwound);
                    }
                    continue;
                }
                continue;
            }

            if ($stmt instanceof Expression) {
                // Try to convert expect() chains
                if (self::isExpectChain($stmt->expr)) {
                    $rewritten = self::rewriteExpectChain($stmt->expr, $subjectVar, $paramMap);
                    $unwound = ExpectChainUnwinder::unwind($rewritten);
                    if ($unwound !== null) {
                        array_push($stmts, ...$unwound);
                        continue;
                    }
                }

                // Pass through with substitution
                $stmts[] = new Expression(self::substituteExpr($stmt->expr, $subjectVar, $paramMap));
                continue;
            }

            // Other statements — substitute and pass through
            $stmts[] = self::substituteStmt($stmt, $subjectVar, $paramMap);
        }

        return $stmts;
    }

    /**
     * Check if an expression is a $this->toXxx()... chain (delegation to built-in expectations).
     */
    private static function isThisExpectChain(Expr $expr): bool
    {
        $current = $expr;
        while ($current instanceof MethodCall || $current instanceof PropertyFetch) {
            $current = $current->var;
        }
        return $current instanceof Variable && $current->name === 'this';
    }

    /**
     * Check if an expression is an expect(...)... chain.
     */
    private static function isExpectChain(Expr $expr): bool
    {
        $current = $expr;
        while ($current instanceof MethodCall || $current instanceof PropertyFetch) {
            $current = $current->var;
        }
        return $current instanceof FuncCall
            && $current->name instanceof Name
            && $current->name->toString() === 'expect';
    }

    /**
     * Check if an expression has arbitrary method calls on $this->value
     * (not just reading the property, but calling methods on it).
     */
    private static function hasArbitraryThisValueUsage(Expr $expr): bool
    {
        $finder = new NodeFinder();
        $found = $finder->find($expr, function (Node $node) {
            if (! $node instanceof MethodCall) {
                return false;
            }
            // Check if the object is $this->value or derived from it
            if ($node->var instanceof PropertyFetch
                && $node->var->var instanceof Variable
                && $node->var->var->name === 'this'
                && $node->var->name instanceof Identifier
                && $node->var->name->name === 'value') {
                return true;
            }
            return false;
        });

        return count($found) > 0;
    }

    /**
     * Rewrite $this->toXxx($params)... chain into expect($subject)->toXxx($params)...
     * Substitutes closure params for call-site args.
     *
     * @param array<string, Expr> $paramMap
     */
    private static function rewriteThisChainToExpect(Expr $expr, Expr $subject, array $paramMap): Expr
    {
        // First substitute all parameter references and $this->value
        $expr = self::substituteExpr($expr, $subject, $paramMap);

        // Now replace the root $this with expect($subject)
        return self::replaceThisWithExpect($expr, $subject);
    }

    /**
     * Replace root $this variable in a chain with expect($subject).
     */
    private static function replaceThisWithExpect(Expr $expr, Expr $subject): Expr
    {
        if ($expr instanceof Variable && $expr->name === 'this') {
            return new FuncCall(new Name('expect'), [new Arg($subject)]);
        }

        if ($expr instanceof MethodCall) {
            $newVar = self::replaceThisWithExpect($expr->var, $subject);
            return new MethodCall($newVar, $expr->name, $expr->args);
        }

        if ($expr instanceof PropertyFetch) {
            $newVar = self::replaceThisWithExpect($expr->var, $subject);
            return new PropertyFetch($newVar, $expr->name);
        }

        return $expr;
    }

    /**
     * Rewrite an expect($something) chain, substituting $this->value references
     * in the expect argument and the chain arguments.
     *
     * @param array<string, Expr> $paramMap
     */
    private static function rewriteExpectChain(Expr $expr, Expr $subject, array $paramMap): Expr
    {
        return self::substituteExpr($expr, $subject, $paramMap);
    }

    /**
     * Deep-substitute expressions: replace $this->value with $subject,
     * and closure param variables with their call-site values.
     *
     * @param array<string, Expr> $paramMap
     */
    private static function substituteExpr(Expr $expr, Expr $subject, array $paramMap): Expr
    {
        $traverser = new NodeTraverser();
        $traverser->addVisitor(new class ($subject, $paramMap) extends NodeVisitorAbstract {
            /** @param array<string, Expr> $paramMap */
            public function __construct(
                private readonly Expr $subject,
                private readonly array $paramMap,
            ) {}

            public function leaveNode(Node $node): ?Node
            {
                // $this->value → $subject
                if ($node instanceof PropertyFetch
                    && $node->var instanceof Variable
                    && $node->var->name === 'this'
                    && $node->name instanceof Identifier
                    && $node->name->name === 'value') {
                    return clone $this->subject;
                }

                // $paramName → callsite arg value
                if ($node instanceof Variable && is_string($node->name) && isset($this->paramMap[$node->name])) {
                    return clone $this->paramMap[$node->name];
                }

                return null;
            }
        });

        $result = $traverser->traverse([new Expression($expr)]);
        if (isset($result[0]) && $result[0] instanceof Expression) {
            return $result[0]->expr;
        }
        return $expr;
    }

    /**
     * Deep-substitute a statement.
     *
     * @param array<string, Expr> $paramMap
     */
    private static function substituteStmt(Stmt $stmt, Expr $subject, array $paramMap): Stmt
    {
        $traverser = new NodeTraverser();
        $traverser->addVisitor(new class ($subject, $paramMap) extends NodeVisitorAbstract {
            /** @param array<string, Expr> $paramMap */
            public function __construct(
                private readonly Expr $subject,
                private readonly array $paramMap,
            ) {}

            public function leaveNode(Node $node): ?Node
            {
                // $this->value → $subject
                if ($node instanceof PropertyFetch
                    && $node->var instanceof Variable
                    && $node->var->name === 'this'
                    && $node->name instanceof Identifier
                    && $node->name->name === 'value') {
                    return clone $this->subject;
                }

                // $paramName → callsite arg value
                if ($node instanceof Variable && is_string($node->name) && isset($this->paramMap[$node->name])) {
                    return clone $this->paramMap[$node->name];
                }

                return null;
            }
        });

        $result = $traverser->traverse([$stmt]);
        return $result[0] ?? $stmt;
    }

    /**
     * Deep clone a list of statements.
     *
     * @param list<Stmt> $stmts
     * @return list<Stmt>
     */
    private static function cloneStmts(array $stmts): array
    {
        $traverser = new NodeTraverser();
        $cloned = $traverser->traverse($stmts);
        /** @var list<Stmt> $cloned */
        return $cloned;
    }
}
