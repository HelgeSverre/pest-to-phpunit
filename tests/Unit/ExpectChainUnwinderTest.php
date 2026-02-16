<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Tests\Unit;

use HelgeSverre\PestToPhpUnit\Helper\ExpectChainUnwinder;
use PhpParser\Node\Arg;
use PhpParser\Node\Expr;
use PhpParser\Node\Expr\ArrowFunction;
use PhpParser\Node\Expr\ClassConstFetch;
use PhpParser\Node\Expr\Closure;
use PhpParser\Node\Expr\FuncCall;
use PhpParser\Node\Expr\MethodCall;
use PhpParser\Node\Expr\PropertyFetch;
use PhpParser\Node\Expr\Variable;
use PhpParser\Node\Identifier;
use PhpParser\Node\Name;
use PhpParser\Node\Param;
use PhpParser\Node\Scalar\Int_;
use PhpParser\Node\Scalar\String_;
use PhpParser\Node\Stmt;
use PhpParser\PrettyPrinter\Standard as PrettyPrinter;
use PHPUnit\Framework\Attributes\DataProvider;
use PHPUnit\Framework\TestCase;

final class ExpectChainUnwinderTest extends TestCase
{
    private static PrettyPrinter $printer;

    public static function setUpBeforeClass(): void
    {
        self::$printer = new PrettyPrinter();
    }

    private static function expect(Expr $subject): FuncCall
    {
        return new FuncCall(new Name('expect'), [new Arg($subject)]);
    }

    private static function chain(Expr $expr, string $name, array $args = []): MethodCall
    {
        $argNodes = array_map(fn ($a) => $a instanceof Arg ? $a : new Arg($a), $args);
        return new MethodCall($expr, new Identifier($name), $argNodes);
    }

    private static function prop(Expr $expr, string $name): PropertyFetch
    {
        return new PropertyFetch($expr, new Identifier($name));
    }

    private static function unwindAndPrint(Expr $expr): ?string
    {
        $stmts = ExpectChainUnwinder::unwind($expr);
        if ($stmts === null) {
            return null;
        }
        return self::$printer->prettyPrint($stmts);
    }

    private function assertUnwindContains(Expr $expr, string $expected, string $message = ''): void
    {
        $result = self::unwindAndPrint($expr);
        $this->assertNotNull($result, 'unwind() returned null' . ($message ? ": {$message}" : ''));
        $this->assertStringContainsString($expected, $result, $message);
    }

    private function assertUnwindNotContains(Expr $expr, string $notExpected, string $message = ''): void
    {
        $result = self::unwindAndPrint($expr);
        $this->assertNotNull($result, 'unwind() returned null' . ($message ? ": {$message}" : ''));
        $this->assertStringNotContainsString($notExpected, $result, $message);
    }

    // Smoke test: basic expect($x)->toBe(1) works
    public function test_basic_expect_to_be(): void
    {
        $expr = self::chain(self::expect(new Variable('x')), 'toBe', [new Int_(1)]);
        $this->assertUnwindContains($expr, 'assertSame');
    }

    /**
     * @return array<string, array{string, string}>
     */
    public static function eachModeMapProvider(): array
    {
        return [
            'toBe' => ['toBe', 'expected_actual'],
            'toEqual' => ['toEqual', 'expected_actual'],
            'toBeNull' => ['toBeNull', 'actual_only'],
            'toBeTrue' => ['toBeTrue', 'actual_only'],
            'toContain' => ['toContain', 'expected_actual'],
            'toBeInstanceOf' => ['toBeInstanceOf', 'expected_actual'],
            'toBeString' => ['toBeString', 'actual_only'],
            'toBeGreaterThan' => ['toBeGreaterThan', 'expected_actual'],
        ];
    }

    #[DataProvider('eachModeMapProvider')]
    public function test_each_mode_produces_foreach(string $pestMethod, string $argOrder): void
    {
        $expr = self::prop(self::expect(new Variable('items')), 'each');
        $args = $argOrder === 'expected_actual' ? [new Int_(1)] : [];
        $expr = self::chain($expr, $pestMethod, $args);

        $result = self::unwindAndPrint($expr);
        $this->assertNotNull($result);
        $this->assertStringContainsString('foreach', $result, "each->{$pestMethod}() should produce foreach");
        $this->assertStringContainsString('__pest_each_item', $result);
    }

    /**
     * @return array<string, array{string, string, string}>
     */
    public static function mapNegationProvider(): array
    {
        $cases = [];
        foreach (\HelgeSverre\PestToPhpUnit\Mapping\ExpectationMethodMap::MAP as $pestMethod => [$phpunitMethod, $argOrder]) {
            $cases[$pestMethod] = [$pestMethod, $phpunitMethod, $argOrder];
        }
        return $cases;
    }

    #[DataProvider('mapNegationProvider')]
    public function test_negated_map_entry_produces_output(string $pestMethod, string $phpunitMethod, string $argOrder): void
    {
        $expr = self::prop(self::expect(new Variable('x')), 'not');
        $args = $argOrder === 'expected_actual' ? [new Int_(1)] : [];
        $expr = self::chain($expr, $pestMethod, $args);

        $result = self::unwindAndPrint($expr);
        $this->assertNotNull($result, "not->{$pestMethod}() returned null");

        $negated = \HelgeSverre\PestToPhpUnit\Mapping\ExpectationMethodMap::getNegated($phpunitMethod);
        if ($negated !== null) {
            $this->assertStringContainsString($negated, $result, "not->{$pestMethod}() should use {$negated}");
        } else {
            $this->assertStringContainsString('TODO', $result, "not->{$pestMethod}() has no negation — should produce TODO");
        }
    }

    public function test_tap_closure_no_params(): void
    {
        $tap = new Closure(['stmts' => [new Stmt\Expression(new FuncCall(new Name('doStuff')))]]);
        $expr = self::chain(self::chain(self::expect(new Variable('x')), 'tap', [$tap]), 'toBe', [new Int_(1)]);
        $result = self::unwindAndPrint($expr);
        $this->assertNotNull($result);
        $this->assertStringContainsString('doStuff', $result);
        $this->assertStringContainsString('assertSame', $result);
    }

    public function test_tap_closure_with_param(): void
    {
        $tap = new Closure([
            'params' => [new Param(new Variable('v'))],
            'stmts' => [new Stmt\Expression(
                self::chain(self::expect(new Variable('v')), 'toBeInt')
            )],
        ]);
        $expr = self::chain(self::expect(new Variable('x')), 'tap', [$tap]);
        $result = self::unwindAndPrint($expr);
        $this->assertNotNull($result);
        $this->assertStringContainsString('$v = $x', $result);
        $this->assertStringContainsString('assertIsInt', $result);
    }

    public function test_tap_arrow_with_param(): void
    {
        $tap = new ArrowFunction([
            'params' => [new Param(new Variable('v'))],
            'expr' => self::chain(self::expect(new Variable('v')), 'toBeInt'),
        ]);
        $expr = self::chain(self::expect(new Variable('x')), 'tap', [$tap]);
        $result = self::unwindAndPrint($expr);
        $this->assertNotNull($result);
        $this->assertStringContainsString('$v = $x', $result);
        $this->assertStringContainsString('assertIsInt', $result);
    }

    public function test_tap_arrow_no_param(): void
    {
        $tap = new ArrowFunction(['expr' => new FuncCall(new Name('doStuff'))]);
        $expr = self::chain(self::chain(self::expect(new Variable('x')), 'tap', [$tap]), 'toBe', [new Int_(1)]);
        $result = self::unwindAndPrint($expr);
        $this->assertNotNull($result);
        $this->assertStringContainsString('doStuff', $result);
        $this->assertStringContainsString('assertSame', $result);
    }

    public function test_to_throw_no_args(): void
    {
        $closure = new ArrowFunction(['expr' => new FuncCall(new Name('x'))]);
        $chain = self::chain(self::expect($closure), 'toThrow');
        $this->assertUnwindContains($chain, 'expectException');
        $this->assertUnwindContains($chain, 'Throwable');
    }

    public function test_to_throw_class_const_fetch(): void
    {
        $closure = new ArrowFunction(['expr' => new FuncCall(new Name('x'))]);
        $classRef = new ClassConstFetch(new Name('RuntimeException'), new Identifier('class'));
        $chain = self::chain(self::expect($closure), 'toThrow', [$classRef]);
        $this->assertUnwindContains($chain, 'expectException');
        $this->assertUnwindNotContains($chain, 'expectExceptionMessage');
    }

    public function test_to_throw_class_const_with_message(): void
    {
        $closure = new ArrowFunction(['expr' => new FuncCall(new Name('x'))]);
        $classRef = new ClassConstFetch(new Name('RuntimeException'), new Identifier('class'));
        $chain = self::chain(self::expect($closure), 'toThrow', [$classRef, new String_('boom')]);
        $this->assertUnwindContains($chain, 'expectException');
        $this->assertUnwindContains($chain, 'expectExceptionMessage');
    }

    public function test_to_throw_new_instance(): void
    {
        $closure = new ArrowFunction(['expr' => new FuncCall(new Name('x'))]);
        $newExpr = new Expr\New_(new Name('RuntimeException'), [new Arg(new String_('boom'))]);
        $chain = self::chain(self::expect($closure), 'toThrow', [$newExpr]);
        $this->assertUnwindContains($chain, 'expectException');
        $this->assertUnwindContains($chain, 'expectExceptionMessage');
    }

    public function test_to_throw_negated(): void
    {
        $closure = new ArrowFunction(['expr' => new FuncCall(new Name('x'))]);
        $expr = self::chain(self::prop(self::expect($closure), 'not'), 'toThrow');
        $this->assertUnwindContains($expr, 'try');
        $this->assertUnwindContains($expr, 'catch');
        $this->assertUnwindContains($expr, 'fail');
    }

    public function test_to_throw_string_class_with_message(): void
    {
        $closure = new ArrowFunction(['expr' => new FuncCall(new Name('x'))]);
        $chain = self::chain(self::expect($closure), 'toThrow', [
            new String_('RuntimeException'),
            new String_('some message'),
        ]);
        $this->assertUnwindContains($chain, 'expectException');
        $this->assertUnwindContains($chain, 'expectExceptionMessage');
    }

    /**
     * @return array<string, array{string, string}>
     */
    public static function toThrowStringClassificationProvider(): array
    {
        return [
            'short class: RuntimeException' => ['RuntimeException', 'expectException'],
            'short class: TypeError' => ['TypeError', 'expectException'],
            'short class: DivisionByZeroError' => ['DivisionByZeroError', 'expectException'],
            'short class: InvalidArgumentException' => ['InvalidArgumentException', 'expectException'],
            'short class: CustomError' => ['CustomError', 'expectException'],
            'short class: LogicException' => ['LogicException', 'expectException'],
            'FQN: App\\Exceptions\\Custom' => ['App\\Exceptions\\Custom', 'expectException'],
            'FQN: \\RuntimeException' => ['\\RuntimeException', 'expectException'],
            'FQN: App\\Error' => ['App\\Error', 'expectException'],
            'message: something failed' => ['something failed', 'expectExceptionMessage'],
            'message: An Error' => ['An Error', 'expectExceptionMessage'],
            'message: Caught a RuntimeException' => ['Caught a RuntimeException', 'expectExceptionMessage'],
            'message: error occurred' => ['error occurred', 'expectExceptionMessage'],
            'message: Error in processing' => ['Error in processing', 'expectExceptionMessage'],
            'message: empty string' => ['', 'expectExceptionMessage'],
            'message: 123' => ['123', 'expectExceptionMessage'],
        ];
    }

    #[DataProvider('toThrowStringClassificationProvider')]
    public function test_to_throw_string_classification(string $input, string $expectedMethod): void
    {
        $closure = new ArrowFunction(['expr' => new FuncCall(new Name('x'))]);
        $chain = self::chain(self::expect($closure), 'toThrow', [new String_($input)]);
        $this->assertUnwindContains($chain, $expectedMethod, "Wrong classification for '{$input}'");
    }

    /**
     * @return array<string, array{string, list<Expr>, bool, bool, string, string}>
     */
    public static function specialCaseModifierProvider(): array
    {
        return [
            'toHaveLength plain' => ['toHaveLength', [new Int_(3)], false, false, 'assertSame', 'foreach'],
            'toHaveLength negated' => ['toHaveLength', [new Int_(3)], true, false, 'assertNotSame', 'foreach'],
            'toHaveLength each' => ['toHaveLength', [new Int_(3)], false, true, 'foreach', ''],
            'toHaveLength each+negated' => ['toHaveLength', [new Int_(3)], true, true, 'assertNotSame', ''],
            'toMatchArray plain' => ['toMatchArray', [new Variable('e')], false, false, 'assertEquals', 'Not'],
            'toMatchArray negated' => ['toMatchArray', [new Variable('e')], true, false, 'assertNotEquals', ''],
            'toBeBetween plain' => ['toBeBetween', [new Int_(1), new Int_(10)], false, false, 'assertGreaterThanOrEqual', ''],
            'toBeBetween negated' => ['toBeBetween', [new Int_(1), new Int_(10)], true, false, 'assertLessThan', ''],
            'toHaveProperty plain' => ['toHaveProperty', [new String_('name')], false, false, 'assertObjectHasProperty', ''],
            'toHaveProperty negated' => ['toHaveProperty', [new String_('name')], true, false, 'assertObjectNotHasProperty', ''],
        ];
    }

    #[DataProvider('specialCaseModifierProvider')]
    public function test_special_case_modifiers(
        string $method, array $args, bool $negated, bool $each,
        string $mustContain, string $mustNotContain,
    ): void {
        $expr = self::expect(new Variable('subject'));
        if ($each) {
            $expr = self::prop($expr, 'each');
        }
        if ($negated) {
            $expr = self::prop($expr, 'not');
        }
        $argNodes = array_map(fn ($a) => $a instanceof Arg ? $a : new Arg($a), $args);
        $expr = self::chain($expr, $method, $argNodes);

        $result = self::unwindAndPrint($expr);
        $this->assertNotNull($result);
        $this->assertStringContainsString($mustContain, $result);
        if ($mustNotContain !== '') {
            $this->assertStringNotContainsString($mustNotContain, $result);
        }
    }
}
