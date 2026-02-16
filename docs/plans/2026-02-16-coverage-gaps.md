# Coverage Gaps Fix Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Fix 7 coverage gaps and add output syntax validation to the Pest-to-PHPUnit converter.

**Architecture:** Each fix is independent. Describe-scoped hooks get inlined into test methods with try/finally for afterEach. Cross-join datasets generate a composed provider method. Output validation uses PHP-Parser to parse generated code in the test suite.

**Tech Stack:** PHP 8.2+, nikic/php-parser, Rector, PHPUnit 11

---

### Task 1: Describe-scoped beforeEach/afterEach hooks

**Files:**

- Modify: `src/Rector/PestFileToPhpUnitClassRector.php` — `processDescribe()` method (lines 790-843)
- Create: `tests/Rector/Fixture/describe_scoped_before_each.php.inc`
- Create: `tests/Rector/Fixture/describe_scoped_after_each.php.inc`
- Create: `tests/Rector/Fixture/describe_nested_scoped_hooks.php.inc`

**Implementation:**

- In `processDescribe()`, recognize `beforeEach`/`afterEach`/`beforeAll`/`afterAll` calls alongside `test`/`it`/`describe`.
- Pass a scope context array `['beforeEach' => list<list<Stmt>>, 'afterEach' => list<list<Stmt>>]` down recursion.
- In `processTestCall()`, accept optional scoped hooks. Prepend beforeEach bodies (outer-to-inner order). Wrap body+afterEach in try/finally (inner-to-outer order) if afterEach hooks exist.
- For `beforeAll`/`afterAll` inside describe: emit a TODO comment (no clean PHPUnit equivalent without multiple classes).

---

### Task 2: Fix `not->toThrow()` to produce a real assertion

**Files:**

- Modify: `src/Helper/ExpectChainUnwinder.php` — `buildToThrow()` method (lines 473-515)
- Modify: `tests/Rector/Fixture/not_to_throw.php.inc`

**Implementation:**
Replace the negated branch in `buildToThrow()` to emit:

```php
try {
    ($callable)();
    $this->addToAssertionCount(1);
} catch (\Throwable $__exception) {
    $this->fail('Expected no exception, but ' . get_class($__exception) . ' was thrown: ' . $__exception->getMessage());
}
```

When a specific exception class is provided, only fail on that type and rethrow others.

---

### Task 3: Multiple `->with()` cross-join datasets

**Files:**

- Modify: `src/Rector/PestFileToPhpUnitClassRector.php` — `processTestCall()` method (around line 476-487)
- Create: `tests/Rector/Fixture/multiple_with_cross_join.php.inc`
- Create: `tests/Rector/Fixture/multiple_with_named_cross_join.php.inc`

**Implementation:**

- Collect all `with` modifiers for a test instead of processing them one-at-a-time.
- If 1 `with`: existing behavior.
- If N>1: generate individual provider methods for each, then generate a composed cross-join provider that iterates via nested foreach loops and yields merged args. Reference only the composed provider via `#[DataProvider]`.

---

### Task 4: Fix `->pipe()` to transform the subject

**Files:**

- Modify: `src/Helper/ExpectChainUnwinder.php` — `processGroup()` method (lines 185-189)
- Modify: `tests/Rector/Fixture/when_unless_pipe.php.inc`

**Implementation:**
Replace the `pipe` handling to transform `currentSubject`:

```php
if ($name === 'pipe') {
    if (count($args) >= 1) {
        $currentSubject = new FuncCall($args[0]->value, [new Arg($currentSubject)]);
    }
    continue;
}
```

Remove `pipe` from the TODO-emitting `when`/`unless`/`pipe` block.

---

### Task 5: Fix higher-order arch test fixture

**Files:**

- Modify: `tests/Rector/Fixture/higher_order_arch.php.inc`

**Implementation:**
Add expected output separator and expected output (skipped test with TODO comment), consistent with `processArch()` behavior.

---

### Task 6: Fix `toHaveLength` for strings

**Files:**

- Modify: `src/Helper/ExpectChainUnwinder.php` — add custom handling before the generic mapping lookup
- Modify: `src/Mapping/ExpectationMethodMap.php` — remove `toHaveLength` from MAP
- Modify: `tests/Rector/Fixture/to_have_length.php.inc`
- Create: `tests/Rector/Fixture/to_have_length_string.php.inc`
- Create: `tests/Rector/Fixture/to_have_length_negated.php.inc`

**Implementation:**
Handle `toHaveLength` as a special case that emits:

```php
if (is_string($subject)) {
    $this->assertSame($expected, strlen($subject));
} else {
    $this->assertCount($expected, $subject);
}
```

---

### Task 7: Output syntax validation in test suite

**Files:**

- Modify: `tests/Rector/PestFileToPhpUnitClassRectorTest.php`

**Implementation:**
After `$this->doTestFile($filePath)`, extract the expected output from the fixture (everything after `-----`), parse it with `PhpParser\ParserFactory`, and assert no parse errors occurred.

---
