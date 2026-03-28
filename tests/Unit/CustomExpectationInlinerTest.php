<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Tests\Unit;

use HelgeSverre\PestToPhpUnit\Helper\CustomExpectationInliner;
use HelgeSverre\PestToPhpUnit\Helper\CustomExpectationRegistry;
use HelgeSverre\PestToPhpUnit\Model\CustomExpectation;
use PhpParser\Node\Arg;
use PhpParser\Node\Expr\Assign;
use PhpParser\Node\Expr\MethodCall;
use PhpParser\Node\Expr\Variable;
use PhpParser\Node\Identifier;
use PhpParser\Node\Param;
use PhpParser\Node\Scalar\Int_;
use PhpParser\Node\Stmt;
use PhpParser\Node\Stmt\Expression;
use PhpParser\Node\Stmt\If_;
use PhpParser\PrettyPrinter\Standard as PrettyPrinter;
use PHPUnit\Framework\TestCase;

final class CustomExpectationInlinerTest extends TestCase
{
    private static PrettyPrinter $printer;

    public static function setUpBeforeClass(): void
    {
        self::$printer = new PrettyPrinter();
    }

    protected function setUp(): void
    {
        CustomExpectationRegistry::clear();
    }

    protected function tearDown(): void
    {
        CustomExpectationRegistry::clear();
    }

    /**
     * Helper: build $this->methodName(...$args)
     */
    private static function thisMethodCall(string $method, array $args = []): MethodCall
    {
        $argNodes = array_map(fn ($a) => $a instanceof Arg ? $a : new Arg($a), $args);

        return new MethodCall(new Variable('this'), new Identifier($method), $argNodes);
    }

    /**
     * Inline and pretty-print the result.
     *
     * @param list<Stmt> $result
     */
    private static function printStmts(array $result): string
    {
        return self::$printer->prettyPrint($result);
    }

    public function test_delegate_body_inlines_assertion(): void
    {
        // Custom expectation body: $this->toBeGreaterThan(0)
        $body = [
            new Expression(self::thisMethodCall('toBeGreaterThan', [new Int_(0)])),
        ];

        $expectation = new CustomExpectation(
            name: 'toBePositive',
            params: [],
            body: $body,
            isArrow: false,
        );

        $subject = new Variable('value');
        $result = CustomExpectationInliner::inline($expectation, $subject, []);

        $output = self::printStmts($result);
        $this->assertStringContainsString('assertGreaterThan', $output);
        $this->assertStringContainsString('0', $output);
        $this->assertStringContainsString('$value', $output);
    }

    public function test_delegate_body_with_parameter_substitution(): void
    {
        // Custom expectation: function($expected) { $this->toBe($expected); }
        // Called with arg 42
        $body = [
            new Expression(self::thisMethodCall('toBe', [new Variable('expected')])),
        ];

        $expectation = new CustomExpectation(
            name: 'toBeExactly',
            params: [new Param(new Variable('expected'))],
            body: $body,
            isArrow: false,
        );

        $subject = new Variable('actual');
        $callArgs = [new Arg(new Int_(42))];
        $result = CustomExpectationInliner::inline($expectation, $subject, $callArgs);

        $output = self::printStmts($result);
        $this->assertStringContainsString('assertSame', $output);
        $this->assertStringContainsString('42', $output);
        $this->assertStringContainsString('$actual', $output);
    }

    public function test_negated_delegate_body(): void
    {
        // Custom expectation body: $this->toBeGreaterThan(0)
        // With negated = true
        $body = [
            new Expression(self::thisMethodCall('toBeGreaterThan', [new Int_(0)])),
        ];

        $expectation = new CustomExpectation(
            name: 'toBePositive',
            params: [],
            body: $body,
            isArrow: false,
        );

        $subject = new Variable('value');
        $result = CustomExpectationInliner::inline($expectation, $subject, [], negated: true);

        $output = self::printStmts($result);
        // Negated toBeGreaterThan should produce assertLessThanOrEqual
        $this->assertStringContainsString('assertLessThanOrEqual', $output);
        $this->assertStringContainsString('$value', $output);
    }

    public function test_arrow_function_body_inlines_correctly(): void
    {
        // Arrow function: fn() => $this->toBeString()
        // Arrow bodies are wrapped in Expression by the registry
        $body = [
            new Expression(self::thisMethodCall('toBeString')),
        ];

        $expectation = new CustomExpectation(
            name: 'toBeStringType',
            params: [],
            body: $body,
            isArrow: true,
        );

        $subject = new Variable('input');
        $result = CustomExpectationInliner::inline($expectation, $subject, []);

        $output = self::printStmts($result);
        $this->assertStringContainsString('assertIsString', $output);
        $this->assertStringContainsString('$input', $output);
    }

    public function test_complex_body_emits_todo_comment(): void
    {
        // Body with an if statement — classified as complex
        $body = [
            new If_(
                new Variable('condition'),
                [
                    'stmts' => [
                        new Expression(self::thisMethodCall('toBeTrue')),
                    ],
                ]
            ),
        ];

        $expectation = new CustomExpectation(
            name: 'toBeConditional',
            params: [],
            body: $body,
            isArrow: false,
        );

        $subject = new Variable('val');
        $result = CustomExpectationInliner::inline($expectation, $subject, []);

        $output = self::printStmts($result);
        $this->assertStringContainsString('TODO', $output);
        $this->assertStringContainsString('toBeConditional', $output);
    }

    public function test_mixed_body_with_local_variable_and_expect_chain(): void
    {
        // Body:
        //   $len = $this->value;
        //   $this->toBeGreaterThan(0);
        $body = [
            new Expression(
                new Assign(
                    new Variable('len'),
                    new \PhpParser\Node\Expr\PropertyFetch(
                        new Variable('this'),
                        new Identifier('value')
                    )
                )
            ),
            new Expression(self::thisMethodCall('toBeGreaterThan', [new Int_(0)])),
        ];

        $expectation = new CustomExpectation(
            name: 'toHavePositiveLength',
            params: [],
            body: $body,
            isArrow: false,
        );

        $subject = new Variable('str');
        $result = CustomExpectationInliner::inline($expectation, $subject, []);

        $output = self::printStmts($result);
        // $this->value should be substituted with $str
        $this->assertStringContainsString('$len', $output);
        $this->assertStringContainsString('$str', $output);
        $this->assertStringContainsString('assertGreaterThan', $output);
    }
}
