# Correctness Fixes Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Fix 7 issues that produce silently wrong, broken, or incomplete PHPUnit output.

**Architecture:** Each task is independent — a targeted fix in `ExpectChainUnwinder.php` or `ExpectationMethodMap.php` with a corresponding test fixture. TDD: write fixture first (red), implement fix (green).

**Tech Stack:** PHP 8.2+, nikic/php-parser, Rector, PHPUnit 11

**Test command:** `vendor/bin/phpunit tests/Rector/PestFileToPhpUnitClassRectorTest.php`

---

### Task 1: `toThrow()` with no args → `expectException(\Throwable::class)` 🔴 HIGH

**Problem:** `expect(fn() => ...)->toThrow()` invokes the callable but sets NO exception expectation. The test passes even if nothing throws — a **false positive**.

**Files:**

- Modify: `src/Helper/ExpectChainUnwinder.php` — `buildToThrow()` method (~line 649)
- Create: `tests/Rector/Fixture/to_throw_no_args.php.inc`

**Step 1: Create failing fixture**

```
tests/Rector/Fixture/to_throw_no_args.php.inc
```

```php
<?php
test('throws something', function () {
    expect(fn () => riskyOperation())->toThrow();
});
-----
<?php
class ToThrowNoArgsTest extends \PHPUnit\Framework\TestCase
{
    public function test_throws_something(): void
    {
        $this->expectException(\Throwable::class);
        (fn () => riskyOperation())();
    }
}
```

**Step 2: Run test, verify it fails**

**Step 3: Fix `buildToThrow()`**

In `buildToThrow()`, the non-negated path currently has `if (count($args) >= 1)` which skips everything when there are no args, going straight to invoking the callable. Add before that block:

```php
if (count($args) === 0) {
    $stmts[] = new Stmt\Expression(
        new MethodCall(new Variable('this'), 'expectException', [
            new Arg(new ClassConstFetch(new FullyQualified('Throwable'), new Identifier('class'))),
        ])
    );
}
```

**Step 4: Run tests, verify all pass**

**Step 5: Commit**

---

### Task 2: `toThrow('string')` — disambiguate class-string vs message 🟡 MEDIUM

**Problem:** `toThrow('RuntimeException')` becomes `expectExceptionMessage('RuntimeException')` instead of `expectException(RuntimeException::class)`. Pest treats strings ending in `Exception` or `Error` (or containing `\`) as class names.

**Files:**

- Modify: `src/Helper/ExpectChainUnwinder.php` — `buildToThrow()` method (~line 652)
- Create: `tests/Rector/Fixture/to_throw_string_class.php.inc`

**Step 1: Create failing fixture**

```
tests/Rector/Fixture/to_throw_string_class.php.inc
```

```php
<?php
test('throws by class string', function () {
    expect(fn () => fail())->toThrow('RuntimeException');
});

test('throws by fqn string', function () {
    expect(fn () => fail())->toThrow('App\Exceptions\CustomException');
});

test('throws by message string', function () {
    expect(fn () => fail())->toThrow('something went wrong');
});
-----
<?php
class ToThrowStringClassTest extends \PHPUnit\Framework\TestCase
{
    public function test_throws_by_class_string(): void
    {
        $this->expectException(RuntimeException::class);
        (fn () => fail())();
    }
    public function test_throws_by_fqn_string(): void
    {
        $this->expectException(\App\Exceptions\CustomException::class);
        (fn () => fail())();
    }
    public function test_throws_by_message_string(): void
    {
        $this->expectExceptionMessage('something went wrong');
        (fn () => fail())();
    }
}
```

**Step 2: Run test, verify it fails**

**Step 3: Fix `buildToThrow()`**

Replace the `String_` handling block (~line 652):

```php
if ($firstArgValue instanceof String_) {
    $strVal = $firstArgValue->value;
    // Heuristic: if string looks like a class name (ends with Exception/Error, or contains \), treat as class
    if (preg_match('/(?:Exception|Error)$/', $strVal) || str_contains($strVal, '\\')) {
        // Class-string: toThrow('RuntimeException') or toThrow('App\Exceptions\Foo')
        if (str_contains($strVal, '\\')) {
            $classRef = new Arg(new ClassConstFetch(new FullyQualified(ltrim($strVal, '\\')), new Identifier('class')));
        } else {
            $classRef = new Arg(new ClassConstFetch(new Name($strVal), new Identifier('class')));
        }
        $stmts[] = new Stmt\Expression(
            new MethodCall(new Variable('this'), 'expectException', [$classRef])
        );
    } else {
        // Plain message string
        $stmts[] = new Stmt\Expression(
            new MethodCall(new Variable('this'), 'expectExceptionMessage', [$args[0]])
        );
    }
}
```

Also handle the second arg for class-string + message: after the class-string block, if `count($args) >= 2`, emit `expectExceptionMessage` for the second arg.

**Step 4: Run tests, verify all pass (including existing to_throw fixtures)**

**Step 5: Commit**

---

### Task 3: `toHaveLength()` — handle strings vs arrays 🟡 MEDIUM

**Problem:** `toHaveLength` maps to `assertCount` which will TypeError on strings in PHP 8+. Pest's `toHaveLength()` uses `strlen()` for strings and `count()` for countables.

**Files:**

- Modify: `src/Helper/ExpectChainUnwinder.php` — add special case before generic mapping lookup (~line 240)
- Modify: `src/Mapping/ExpectationMethodMap.php` — remove `toHaveLength` from MAP
- Modify: `tests/Rector/Fixture/to_have_length_string.php.inc` — update expected output

**Step 1: Update `to_have_length_string.php.inc` to expect correct output**

Currently expects `assertCount(5, 'hello')` which is wrong. Update to:

```php
<?php
test('string length', function () {
    expect('hello')->toHaveLength(5);
});
-----
<?php
class ToHaveLengthStringTest extends \PHPUnit\Framework\TestCase
{
    public function test_string_length(): void
    {
        $this->assertSame(5, strlen('hello'));
    }
}
```

**Step 2: Run test, verify it fails**

**Step 3: Implement**

Remove `'toHaveLength' => ['assertCount', 'expected_actual']` from `ExpectationMethodMap::MAP`.

In `ExpectChainUnwinder::processGroup()`, add before the generic mapping lookup (~line 240):

```php
if ($name === 'toHaveLength') {
    if (count($args) >= 1) {
        // strlen for strings, count for arrays — use a conditional expression
        // For simplicity: $this->assertSame($expected, is_string($subject) ? strlen($subject) : count($subject))
        $lenExpr = new \PhpParser\Node\Expr\Ternary(
            new FuncCall(new Name('is_string'), [new Arg($currentSubject)]),
            new FuncCall(new Name('strlen'), [new Arg($currentSubject)]),
            new FuncCall(new Name('count'), [new Arg($currentSubject)])
        );
        $phpunitMethod = $negated ? 'assertNotSame' : 'assertSame';
        $negated = false;
        $stmts[] = new Stmt\Expression(
            new MethodCall(new Variable('this'), $phpunitMethod, [
                $args[0],
                new Arg($lenExpr),
            ])
        );
    }
    continue;
}
```

**Step 4: Verify `to_have_length.php.inc` also needs updating** — arrays should still work:

Update `to_have_length.php.inc` expected output to:

```php
$this->assertSame(3, is_string([1, 2, 3]) ? strlen([1, 2, 3]) : count([1, 2, 3]));
```

**Step 5: Update `to_have_length_negated.php.inc` similarly**

**Step 6: Run all tests, verify pass**

**Step 7: Commit**

---

### Task 4: `tap()` with closure parameter — bind to current subject 🟡 MEDIUM

**Problem:** `tap(function ($value) { $value->toBe(42); })` inlines the body but `$value` is undefined — it should be bound to the expect subject.

**Files:**

- Modify: `src/Helper/ExpectChainUnwinder.php` — `tap` handling (~line 196)
- Create: `tests/Rector/Fixture/tap_with_param.php.inc`

**Step 1: Create failing fixture**

```
tests/Rector/Fixture/tap_with_param.php.inc
```

```php
<?php
test('tap with param', function () {
    expect($result)->tap(function ($value) {
        expect($value)->toBeGreaterThan(0);
    })->toBe(42);
});
-----
<?php
class TapWithParamTest extends \PHPUnit\Framework\TestCase
{
    public function test_tap_with_param(): void
    {
        $this->assertGreaterThan(0, $result);
        $this->assertSame(42, $result);
    }
}
```

**Step 2: Run test, verify it fails**

**Step 3: Fix `tap` handling**

Replace the tap block (~line 196):

```php
if ($name === 'tap') {
    if (count($args) >= 1 && $args[0]->value instanceof \PhpParser\Node\Expr\Closure) {
        $tapClosure = $args[0]->value;
        // If closure has a parameter, it receives the current subject
        // Transform the closure body: replace expect($param) with expect($currentSubject)
        if (count($tapClosure->params) >= 1) {
            $paramName = $tapClosure->params[0]->var->name;
            foreach ($tapClosure->stmts as $tapStmt) {
                // Process expect() chains inside tap body by recursively unwinding
                if ($tapStmt instanceof \PhpParser\Node\Stmt\Expression) {
                    $innerResult = self::unwind($tapStmt->expr);
                    if ($innerResult !== null) {
                        // Replace the param variable with current subject in the unwound statements
                        // This is handled by the fact that expect($param) will use $param as subject,
                        // but we need $param = $currentSubject
                        $assignStmt = new \PhpParser\Node\Stmt\Expression(
                            new \PhpParser\Node\Expr\Assign(
                                new Variable($paramName),
                                $currentSubject
                            )
                        );
                        $stmts[] = $assignStmt;
                        array_push($stmts, ...$innerResult);
                    } else {
                        $stmts[] = $tapStmt;
                    }
                } else {
                    $stmts[] = $tapStmt;
                }
            }
        } else {
            // No param — just inline the body
            foreach ($tapClosure->stmts as $tapStmt) {
                $stmts[] = $tapStmt;
            }
        }
    }
    continue;
}
```

Actually, simpler approach: just assign `$paramName = $currentSubject` once, then inline the body. The existing `expect($value)` inside tap will get processed by `transformBody()` at the test method level.

Simplest fix:

```php
if ($name === 'tap') {
    if (count($args) >= 1 && $args[0]->value instanceof \PhpParser\Node\Expr\Closure) {
        $tapClosure = $args[0]->value;
        // If closure has a parameter, assign it to current subject
        if (count($tapClosure->params) >= 1) {
            $paramName = $tapClosure->params[0]->var->name;
            $stmts[] = new \PhpParser\Node\Stmt\Expression(
                new \PhpParser\Node\Expr\Assign(
                    new Variable($paramName),
                    $currentSubject
                )
            );
        }
        foreach ($tapClosure->stmts as $tapStmt) {
            $stmts[] = $tapStmt;
        }
    }
    continue;
}
```

Note: The `expect()` calls inside the tap body will be handled by `transformBody()` in the Rector class (it processes all expect chains in method bodies). So the fixture expected output should show the inner expect being transformed too. Adjust fixture accordingly — the `expect($value)->toBeGreaterThan(0)` inside tap becomes `$this->assertGreaterThan(0, $value)` and `$value = $result` is prepended.

**Step 4: Run tests, verify pass**

**Step 5: Commit**

---

### Task 5: `beforeAll`/`afterAll` inside describe → emit TODO 🟢 LOW (easy)

**Problem:** `beforeAll`/`afterAll` inside describe blocks are silently dropped. They should emit a TODO comment.

**Files:**

- Modify: `src/Rector/PestFileToPhpUnitClassRector.php` — `processDescribe()` (~line 926)
- Create: `tests/Rector/Fixture/describe_before_all_todo.php.inc`

**Step 1: Create failing fixture**

```
tests/Rector/Fixture/describe_before_all_todo.php.inc
```

```php
<?php
describe('Database', function () {
    beforeAll(function () {
        DB::seed();
    });

    afterAll(function () {
        DB::truncate();
    });

    test('has data', function () {
        expect(true)->toBeTrue();
    });
});
-----
<?php
class DescribeBeforeAllTodoTest extends \PHPUnit\Framework\TestCase
{
    public function test_database_has_data(): void
    {
        // TODO(Pest): beforeAll() inside describe() cannot be scoped in PHPUnit — move to setUpBeforeClass() or inline
        // TODO(Pest): afterAll() inside describe() cannot be scoped in PHPUnit — move to tearDownAfterClass() or inline
        $this->assertTrue(true);
    }
}
```

**Step 2: Run test, verify it fails**

**Step 3: Implement**

In `processDescribe()`, after collecting `localBeforeEach`/`localAfterEach` (~line 870-903), also collect `beforeAll`/`afterAll` and generate TODO comments. Add to the hook collection loop:

```php
} elseif ($fn === 'beforeAll') {
    $localBeforeAllFound = true;
} elseif ($fn === 'afterAll') {
    $localAfterAllFound = true;
}
```

Then when processing each test in the describe, prepend TODO Nop comments to the scoped hooks:

```php
$scopedTodos = [];
if ($localBeforeAllFound) {
    $nop = new Nop();
    $nop->setAttribute('comments', [new Comment('// TODO(Pest): beforeAll() inside describe() cannot be scoped in PHPUnit — move to setUpBeforeClass() or inline')]);
    $scopedTodos[] = $nop;
}
if ($localAfterAllFound) {
    $nop = new Nop();
    $nop->setAttribute('comments', [new Comment('// TODO(Pest): afterAll() inside describe() cannot be scoped in PHPUnit — move to tearDownAfterClass() or inline')]);
    $scopedTodos[] = $nop;
}
```

Pass these down and prepend to each test's body in the describe scope.

**Step 4: Run tests, verify pass**

**Step 5: Commit**

---

### Task 6: Fix `assertJson` negation 🟢 LOW (easy)

**Problem:** `not->toBeJson()` maps to `assertIsNotString` which is semantically wrong. Should emit TODO since there's no clean PHPUnit negation.

**Files:**

- Modify: `src/Mapping/ExpectationMethodMap.php` — change `assertJson` negation
- Modify: `src/Helper/ExpectChainUnwinder.php` — handle missing negation with TODO
- Create: `tests/Rector/Fixture/not_to_be_json.php.inc`

**Step 1: Create failing fixture**

```
tests/Rector/Fixture/not_to_be_json.php.inc
```

```php
<?php
test('not json', function () {
    expect($value)->not->toBeJson();
});
-----
<?php
class NotToBeJsonTest extends \PHPUnit\Framework\TestCase
{
    public function test_not_json(): void
    {
        // TODO(Pest): not->toBeJson() has no direct PHPUnit equivalent
    }
}
```

**Step 2: Run test, verify it fails** (currently emits `assertIsNotString` which is wrong)

**Step 3: Implement**

Remove the `'assertJson' => 'assertIsNotString'` line from `ExpectationMethodMap::NEGATED_MAP`.

In `processGroup()`, when `$negated` is true and `ExpectationMethodMap::getNegated()` returns null, emit a TODO comment instead of using the un-negated method:

The current code at ~line 245-248:

```php
if ($negated) {
    $phpunitMethod = ExpectationMethodMap::getNegated($phpunitMethod) ?? $phpunitMethod;
    $negated = false;
}
```

Change to:

```php
if ($negated) {
    $negatedMethod = ExpectationMethodMap::getNegated($phpunitMethod);
    if ($negatedMethod !== null) {
        $phpunitMethod = $negatedMethod;
    } else {
        // No safe negation — emit TODO
        $comment = new Nop();
        $comment->setAttribute('comments', [new Comment("// TODO(Pest): not->{$name}() has no direct PHPUnit equivalent")]);
        $stmts[] = $comment;
        $negated = false;
        continue;
    }
    $negated = false;
}
```

**Step 4: Check that `negated_json_nan.php.inc` fixture still passes** — may need updating since `not->toBeJson()` output will change.

**Step 5: Run all tests, verify pass**

**Step 6: Commit**

---

### Task 7: Fix `toBeUppercase`/`toBeLowercase` regex accuracy 🟢 LOW (easy)

**Problem:** Current regexes `/^[^a-z]*$/` and `/^[^A-Z]*$/` accept empty strings, digits, and punctuation. Pest uses `strtoupper($value) === $value` / `strtolower($value) === $value`.

**Files:**

- Modify: `src/Helper/ExpectChainUnwinder.php` — `toBeUppercase`/`toBeLowercase` handling (~line 440-441)
- Modify: `tests/Rector/Fixture/to_be_uppercase.php.inc` — update expected regex
- Modify: `tests/Rector/Fixture/to_be_lowercase.php.inc` — update expected regex

**Step 1: Update the regexes**

Replace the regex approach with Pest's actual semantics for these two. Instead of a regex match, emit:

For `toBeUppercase`:

```php
$this->assertSame(strtoupper($subject), $subject);
```

For `toBeLowercase`:

```php
$this->assertSame(strtolower($subject), $subject);
```

This is more accurate than any regex. Extract these two from the regex map and handle them separately before the regex block.

**Step 2: Update fixture expected outputs**

**Step 3: Run tests, verify pass**

**Step 4: Commit**

---

## Summary

| Task                                        | Priority | Effort | Impact                             |
| ------------------------------------------- | :------: | :----: | ---------------------------------- |
| 1. `toThrow()` no args                      | 🔴 HIGH  |   S    | Prevents false-positive tests      |
| 2. `toThrow('string')` disambiguation       |  🟡 MED  |   S    | Correct exception class vs message |
| 3. `toHaveLength` string support            |  🟡 MED  |   S    | Prevents TypeError on strings      |
| 4. `tap()` with parameter                   |  🟡 MED  |   M    | Prevents undefined variable        |
| 5. Describe `beforeAll`/`afterAll` TODO     |  🟢 LOW  |   S    | Prevents silent data loss          |
| 6. `assertJson` negation                    |  🟢 LOW  |   S    | Fixes incorrect negation           |
| 7. `toBeUppercase`/`toBeLowercase` accuracy |  🟢 LOW  |   S    | More accurate conversion           |

Total estimated effort: ~2-3 hours
