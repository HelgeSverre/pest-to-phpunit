<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Tests\Unit;

use HelgeSverre\PestToPhpUnit\Helper\CustomExpectationRegistry;
use HelgeSverre\PestToPhpUnit\Model\CustomExpectation;
use PhpParser\Node\Arg;
use PhpParser\Node\Expr\ArrowFunction;
use PhpParser\Node\Expr\Closure;
use PhpParser\Node\Expr\FuncCall;
use PhpParser\Node\Expr\MethodCall;
use PhpParser\Node\Expr\Variable;
use PhpParser\Node\Identifier;
use PhpParser\Node\Name;
use PhpParser\Node\Scalar\Int_;
use PhpParser\Node\Scalar\String_;
use PhpParser\Node\Stmt\Expression;
use PHPUnit\Framework\TestCase;

final class CustomExpectationRegistryTest extends TestCase
{
    protected function setUp(): void
    {
        CustomExpectationRegistry::clear();
    }

    protected function tearDown(): void
    {
        CustomExpectationRegistry::clear();
    }

    public function test_register_and_get_roundtrip(): void
    {
        $expectation = new CustomExpectation(
            name: 'toBePositive',
            params: [],
            body: [],
            isArrow: false,
        );

        CustomExpectationRegistry::register($expectation);

        $retrieved = CustomExpectationRegistry::get('toBePositive');
        $this->assertNotNull($retrieved);
        $this->assertSame('toBePositive', $retrieved->name);
        $this->assertSame([], $retrieved->params);
        $this->assertSame([], $retrieved->body);
    }

    public function test_has_returns_false_for_unregistered(): void
    {
        $this->assertFalse(CustomExpectationRegistry::has('nonexistent'));
    }

    public function test_has_returns_true_after_register(): void
    {
        CustomExpectationRegistry::register(new CustomExpectation(
            name: 'toBeEven',
            params: [],
            body: [],
        ));

        $this->assertTrue(CustomExpectationRegistry::has('toBeEven'));
    }

    public function test_clear_removes_all_expectations(): void
    {
        CustomExpectationRegistry::register(new CustomExpectation('toBeA', [], []));
        CustomExpectationRegistry::register(new CustomExpectation('toBeB', [], []));
        CustomExpectationRegistry::register(new CustomExpectation('toBeC', [], []));

        $this->assertCount(3, CustomExpectationRegistry::all());

        CustomExpectationRegistry::clear();

        $this->assertSame([], CustomExpectationRegistry::all());
    }

    public function test_duplicate_name_overwrites(): void
    {
        $first = new CustomExpectation('toBePositive', [], [], isArrow: false);
        $second = new CustomExpectation('toBePositive', [], [], isArrow: true);

        CustomExpectationRegistry::register($first);
        CustomExpectationRegistry::register($second);

        $retrieved = CustomExpectationRegistry::get('toBePositive');
        $this->assertNotNull($retrieved);
        $this->assertTrue($retrieved->isArrow, 'The latest registered expectation should win');
    }

    public function test_all_returns_all_registered(): void
    {
        CustomExpectationRegistry::register(new CustomExpectation('toBeA', [], []));
        CustomExpectationRegistry::register(new CustomExpectation('toBeB', [], []));
        CustomExpectationRegistry::register(new CustomExpectation('toBeC', [], []));

        $all = CustomExpectationRegistry::all();
        $this->assertCount(3, $all);
        $this->assertArrayHasKey('toBeA', $all);
        $this->assertArrayHasKey('toBeB', $all);
        $this->assertArrayHasKey('toBeC', $all);
    }

    public function test_get_returns_null_for_unknown(): void
    {
        $this->assertNull(CustomExpectationRegistry::get('doesNotExist'));
    }

    public function test_collect_from_statements_with_extend_call(): void
    {
        // Build AST for: expect()->extend('toBePositive', function() { $this->toBeGreaterThan(0); })
        $closure = new Closure([
            'stmts' => [
                new Expression(
                    new MethodCall(
                        new Variable('this'),
                        new Identifier('toBeGreaterThan'),
                        [new Arg(new Int_(0))],
                    ),
                ),
            ],
        ]);

        $extendCall = new MethodCall(
            new FuncCall(new Name('expect')),
            new Identifier('extend'),
            [
                new Arg(new String_('toBePositive')),
                new Arg($closure),
            ],
        );

        $stmts = [new Expression($extendCall)];

        $registered = CustomExpectationRegistry::collectFromStatements($stmts);

        $this->assertSame(['toBePositive'], $registered);
        $this->assertTrue(CustomExpectationRegistry::has('toBePositive'));

        $expectation = CustomExpectationRegistry::get('toBePositive');
        $this->assertNotNull($expectation);
        $this->assertSame('toBePositive', $expectation->name);
        $this->assertFalse($expectation->isArrow);
        $this->assertCount(1, $expectation->body);
    }

    public function test_collect_from_statements_ignores_non_extend(): void
    {
        // Build AST for: expect($x)->toBe(1)
        $toBe = new MethodCall(
            new FuncCall(new Name('expect'), [new Arg(new Variable('x'))]),
            new Identifier('toBe'),
            [new Arg(new Int_(1))],
        );

        $stmts = [new Expression($toBe)];

        $registered = CustomExpectationRegistry::collectFromStatements($stmts);

        $this->assertSame([], $registered);
        $this->assertSame([], CustomExpectationRegistry::all());
    }

    public function test_collect_from_statements_with_arrow_function(): void
    {
        // Build AST for: expect()->extend('toBePos', fn() => $this->toBeGreaterThan(0))
        $arrow = new ArrowFunction([
            'expr' => new MethodCall(
                new Variable('this'),
                new Identifier('toBeGreaterThan'),
                [new Arg(new Int_(0))],
            ),
        ]);

        $extendCall = new MethodCall(
            new FuncCall(new Name('expect')),
            new Identifier('extend'),
            [
                new Arg(new String_('toBePos')),
                new Arg($arrow),
            ],
        );

        $stmts = [new Expression($extendCall)];

        $registered = CustomExpectationRegistry::collectFromStatements($stmts);

        $this->assertSame(['toBePos'], $registered);

        $expectation = CustomExpectationRegistry::get('toBePos');
        $this->assertNotNull($expectation);
        $this->assertTrue($expectation->isArrow);
    }
}
