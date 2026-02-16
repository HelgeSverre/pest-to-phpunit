# Robustness Testing Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Add two layers of testing beyond golden-file fixtures: (1) metamorphic invariant assertions on all fixtures, and (2) data-provider matrix tests that unit-test `ExpectChainUnwinder::unwind()` directly with combinatorial inputs targeting known-fragile code paths.

**Architecture:** Approach 1 adds invariant checks to the existing `PestFileToPhpUnitClassRectorTest`. Approach 2 adds a new `ExpectChainUnwinderTest` that constructs AST nodes directly, feeds them to `unwind()`, and pretty-prints the result for snapshot assertions. No new dependencies required.

**Tech Stack:** PHP 8.2+, nikic/php-parser, PHPUnit 11

**Test command:** `vendor/bin/phpunit tests/Rector/PestFileToPhpUnitClassRectorTest.php` (fixtures) and `vendor/bin/phpunit tests/Unit/ExpectChainUnwinderTest.php` (matrix)

---

## Part 1: Metamorphic Invariant Assertions

### Task 1: Assert no Pest entry points remain in converted output

**Problem:** Current fixtures only check string equality. A fixture could expect wrong output containing `expect(` calls and still pass. We should assert that no Pest-specific function calls survive conversion (unless inside a TODO comment).

**Files:**
- Modify: `tests/Rector/PestFileToPhpUnitClassRectorTest.php`

**Step 1: Add the invariant assertion method**

Add this method to `PestFileToPhpUnitClassRectorTest`:

```php
private function assertNoPestCallsRemain(string $filePath): void
{
    $content = file_get_contents($filePath);
    $parts = explode("-----\n", $content, 2);

    if (count($parts) < 2) {
        return;
    }

    $expectedOutput = trim($parts[1]);
    if ($expectedOutput === '') {
        return;
    }

    // Strip comments so TODO comments don't trigger false positives
    $withoutComments = preg_replace('#//.*$#m', '', $expectedOutput);
    $withoutComments = preg_replace('#/\*.*?\*/#s', '', $withoutComments);

    $pestCalls = ['expect(', 'test(', 'it(', 'describe(', 'beforeEach(', 'afterEach(', 'beforeAll(', 'afterAll('];
    foreach ($pestCalls as $call) {
        $this->assertStringNotContainsString(
            $call,
            $withoutComments,
            "Expected output of " . basename($filePath) . " still contains Pest call: {$call}"
        );
    }
}
```

**Step 2: Wire it into the test method**

```php
public function test(string $filePath): void
{
    $this->doTestFile($filePath);
    $this->assertFixtureOutputIsValidPhp($filePath);
    $this->assertNoPestCallsRemain($filePath);
}
```

**Step 3: Run tests, verify all 311 pass**

**Step 4: Commit**

---

## Part 2: Data-Provider Matrix Tests for ExpectChainUnwinder

### Task 2: Set up the unit test class with AST helpers

**Problem:** We need a test class that can construct `expect()` chains as AST nodes, feed them to `ExpectChainUnwinder::unwind()`, and assert on the pretty-printed output.

**Files:**
- Create: `tests/Unit/ExpectChainUnwinderTest.php`

**Step 1: Create the test class with helper methods**

```php
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
}
```

**Step 2: Verify it loads** — `vendor/bin/phpunit tests/Unit/ExpectChainUnwinderTest.php`

**Step 3: Commit**

---

### Task 3: toThrow() string classification matrix

**Problem:** The `toThrow('string')` heuristic is the most fragile logic. Test every boundary.

**Files:**
- Modify: `tests/Unit/ExpectChainUnwinderTest.php`

**Step 1: Add the data provider and test**

```php
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
```

**Step 2: Run** — `vendor/bin/phpunit tests/Unit/ExpectChainUnwinderTest.php --filter=to_throw_string`

**Step 3: Commit**

---

### Task 4: toThrow() argument shape matrix

**Problem:** toThrow handles 5 argument shapes. Test all systematically.

**Files:**
- Modify: `tests/Unit/ExpectChainUnwinderTest.php`

**Step 1: Add tests**

```php
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
```

**Step 2: Run** — `vendor/bin/phpunit tests/Unit/ExpectChainUnwinderTest.php --filter=to_throw`

**Step 3: Commit**

---

### Task 5: Modifier combination matrix (not, each) for special-case handlers

**Problem:** Special-case handlers each need to handle `$negated` and `$eachMode`. Test that flags are respected.

**Files:**
- Modify: `tests/Unit/ExpectChainUnwinderTest.php`

**Step 1: Add the data provider**

```php
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
```

**Step 2: Run** — `vendor/bin/phpunit tests/Unit/ExpectChainUnwinderTest.php --filter=special_case`

**Step 3: Commit**

---

### Task 6: tap() callable shape matrix

**Problem:** tap() should handle Closure and ArrowFunction, with/without parameters, and recursively unwind inner expect() calls.

**Files:**
- Modify: `tests/Unit/ExpectChainUnwinderTest.php`

**Step 1: Add tests**

```php
public function test_tap_closure_no_params(): void
{
    $tap = new Closure(['stmts' => [new Stmt\Expression(new FuncCall(new Name('doStuff')))]]);
    $expr = self::chain(self::chain(self::expect(new Variable('x')), 'tap', [$tap]), 'toBe', [new Int_(1)]);
    $result = self::unwindAndPrint($expr);
    $this->assertStringContainsString('doStuff', $result);
    $this->assertStringContainsString('assertSame', $result);
}

public function test_tap_closure_with_param(): void
{
    $tap = new Closure([
        'params' => [new Param(new Variable('v'))],
        'stmts' => [new Stmt\Expression(self::chain(self::expect(new Variable('v')), 'toBeInt'))],
    ]);
    $expr = self::chain(self::expect(new Variable('x')), 'tap', [$tap]);
    $result = self::unwindAndPrint($expr);
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
    $this->assertStringContainsString('$v = $x', $result);
    $this->assertStringContainsString('assertIsInt', $result);
}

public function test_tap_arrow_no_param(): void
{
    $tap = new ArrowFunction(['expr' => new FuncCall(new Name('doStuff'))]);
    $expr = self::chain(self::chain(self::expect(new Variable('x')), 'tap', [$tap]), 'toBe', [new Int_(1)]);
    $result = self::unwindAndPrint($expr);
    $this->assertStringContainsString('doStuff', $result);
    $this->assertStringContainsString('assertSame', $result);
}
```

**Step 2: Run** — `vendor/bin/phpunit tests/Unit/ExpectChainUnwinderTest.php --filter=tap`

**Step 3: Commit**

---

### Task 7: Exhaustive MAP negation coverage

**Problem:** Every MAP entry should either have a NEGATED_MAP entry or produce a TODO when negated. Verify programmatically.

**Files:**
- Modify: `tests/Unit/ExpectChainUnwinderTest.php`

**Step 1: Add exhaustive negation test**

```php
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
        $this->assertStringContainsString($negated, $result);
    } else {
        $this->assertStringContainsString('TODO', $result);
    }
}
```

**Step 2: Run** — `vendor/bin/phpunit tests/Unit/ExpectChainUnwinderTest.php --filter=negated_map`

**Step 3: Commit**

---

### Task 8: each mode coverage for generic MAP entries

**Problem:** If any handler is moved from the generic path to a special case, it might lose eachMode support. Verify a representative set.

**Files:**
- Modify: `tests/Unit/ExpectChainUnwinderTest.php`

**Step 1: Add each-mode test**

```php
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
    $this->assertStringContainsString('foreach', $result);
    $this->assertStringContainsString('__each_item', $result);
}
```

**Step 2: Run** — `vendor/bin/phpunit tests/Unit/ExpectChainUnwinderTest.php --filter=each_mode`

**Step 3: Commit**

---

## Summary

| Task | Type | Priority | What it catches |
|---|---|:---:|---|
| 1 | Invariant | HIGH | Pest calls surviving in output |
| 2 | Setup | HIGH | Foundation for all matrix tests |
| 3 | Matrix | HIGH | toThrow string classification boundaries |
| 4 | Matrix | HIGH | toThrow argument shape coverage |
| 5 | Matrix | MED | Special-case handlers × not/each modifiers |
| 6 | Matrix | MED | tap() callable shape variants |
| 7 | Matrix | LOW | Exhaustive negation coverage for all MAP entries |
| 8 | Matrix | LOW | Exhaustive each-mode coverage for MAP entries |

**What these tests would have caught (retroactively):**
- Task 3 → `toThrow('An Error')` misclassification
- Task 6 → `tap(fn() => ...)` ArrowFunction not handled
- Task 5 → `each->toHaveLength()` eachMode ignored
- Task 7 → `not->toBeNan()` self-mapping bug

**Total estimated effort:** ~2 hours
