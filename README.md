# Pest to PHPUnit — Rector Extension

> [!WARNING]
> This project is experimental. It handles many common Pest patterns, but edge cases may produce incorrect output. Always review the generated code before committing.

[![Latest Version on Packagist](https://img.shields.io/packagist/v/helgesverre/pest-to-phpunit.svg?style=flat-square)](https://packagist.org/packages/helgesverre/pest-to-phpunit)
[![Total Downloads](https://img.shields.io/packagist/dt/helgesverre/pest-to-phpunit.svg?style=flat-square)](https://packagist.org/packages/helgesverre/pest-to-phpunit)
[![PHP Version](https://img.shields.io/packagist/php-v/helgesverre/pest-to-phpunit.svg?style=flat-square)](https://packagist.org/packages/helgesverre/pest-to-phpunit)
[![License](https://img.shields.io/packagist/l/helgesverre/pest-to-phpunit.svg?style=flat-square)](LICENSE)

A **Rector extension** that automatically converts **Pest** test files into **PHPUnit** test classes.

Handles `test()` / `it()` blocks, hooks, datasets, `expect()` assertion chains, modifiers, and more — getting you most of the way there automatically while leaving clear `TODO` markers for anything that needs manual review.

## Installation

```bash
composer require --dev helgesverre/pest-to-phpunit
```

## Usage

### 1. Add the set to your `rector.php`

```php
<?php

declare(strict_types=1);

use Rector\Config\RectorConfig;
use HelgeSverre\PestToPhpUnit\Set\PestToPhpUnitSetList;

return RectorConfig::configure()
    ->withSets([
        PestToPhpUnitSetList::PEST_TO_PHPUNIT,
    ]);
```

### 2. Run Rector

```bash
# Preview changes (dry run)
vendor/bin/rector process tests --dry-run

# Apply changes
vendor/bin/rector process tests

# Only a specific folder
vendor/bin/rector process tests/Feature
```

## Examples

### Basic `test()` / `it()`

**Before:**

```php
test('adds numbers', function () {
    expect(1 + 1)->toBe(2);
});

it('subtracts numbers', function () {
    expect(5 - 3)->toBe(2);
});
```

**After:**

```php
class BasicTest extends \PHPUnit\Framework\TestCase
{
    public function test_adds_numbers(): void
    {
        $this->assertSame(2, 1 + 1);
    }
    public function test_it_subtracts_numbers(): void
    {
        $this->assertSame(2, 5 - 3);
    }
}
```

### `describe()` blocks

```php
// Before
describe('Auth', function () {
    it('logs in', function () {
        expect(true)->toBeTrue();
    });
});

// After
class DescribeBlocksTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_auth_logs_in(): void
    {
        $this->assertTrue(true);
    }
}
```

### Hooks → `setUp` / `tearDown`

```php
// Before
beforeEach(function () {
    $this->user = new User();
});

afterEach(function () {
    $this->user = null;
});

test('user exists', function () {
    expect($this->user)->not->toBeNull();
});

// After
class HooksTest extends \PHPUnit\Framework\TestCase
{
    protected $user;
    protected function setUp(): void
    {
        parent::setUp();
        $this->user = new User();
    }
    protected function tearDown(): void
    {
        parent::tearDown();
        $this->user = null;
    }
    public function test_user_exists(): void
    {
        $this->assertNotNull($this->user);
    }
}
```

### `uses()` → extends + traits

```php
// Before
uses(Tests\TestCase::class, RefreshDatabase::class);

test('database works', function () {
    expect(true)->toBeTrue();
});

// After
class UsesTraitsTest extends \Tests\TestCase
{
    use RefreshDatabase;
    public function test_database_works(): void
    {
        $this->assertTrue(true);
    }
}
```

### Modifiers: `skip()`, `todo()`, `throws()`, `group()`

```php
// Before
test('skipped test', function () {
    expect(true)->toBeTrue();
})->skip('Not ready yet');

test('todo test', function () {
})->todo();

test('throws exception', function () {
    throw new RuntimeException('fail');
})->throws(RuntimeException::class, 'fail');

test('grouped test', function () {
    expect(true)->toBeTrue();
})->group('unit');

// After
class ModifiersTest extends \PHPUnit\Framework\TestCase
{
    public function test_skipped_test(): void
    {
        $this->markTestSkipped('Not ready yet');
    }
    public function test_todo_test(): void
    {
        $this->markTestIncomplete('TODO');
    }
    public function test_throws_exception(): void
    {
        $this->expectException(RuntimeException::class);
        $this->expectExceptionMessage('fail');
        throw new RuntimeException('fail');
    }
    #[\PHPUnit\Framework\Attributes\Group('unit')]
    public function test_grouped_test(): void
    {
        $this->assertTrue(true);
    }
}
```

### `expect()->toThrow()`

```php
// Before
test('throws exception', function () {
    expect(fn () => throw new RuntimeException('boom'))
        ->toThrow(RuntimeException::class, 'boom');
});

// After
class ToThrowTest extends \PHPUnit\Framework\TestCase
{
    public function test_throws_exception(): void
    {
        $this->expectException(RuntimeException::class);
        $this->expectExceptionMessage('boom');
        (fn () => throw new RuntimeException('boom'))();
    }
}
```

### Datasets → `#[DataProvider]`

```php
// Before
dataset('emails', [
    'test@example.com',
    'foo@bar.com',
]);

test('validates email', function (string $email) {
    expect($email)->toContain('@');
})->with('emails');

// After
class DatasetTest extends \PHPUnit\Framework\TestCase
{
    public static function emails(): array
    {
        return [
            'test@example.com',
            'foo@bar.com',
        ];
    }

    #[\PHPUnit\Framework\Attributes\DataProvider('emails')]
    public function test_validates_email(string $email): void
    {
        $this->assertContains('@', $email);
    }
}
```

## Feature Support

### Core Constructs

| Pest Feature | Status | PHPUnit Output |
|---|:---:|---|
| `test()` / `it()` | ✅ | `public function test_*(): void` |
| `describe()` (nested, 4+ levels deep) | ✅ | Method name prefixing |
| `beforeEach` / `afterEach` | ✅ | `setUp()` / `tearDown()` |
| `beforeAll` / `afterAll` | ✅ | `setUpBeforeClass()` / `tearDownAfterClass()` |
| `uses(TestCase::class)` | ✅ | `extends TestCase` |
| `uses(Trait::class)` | ✅ | `use Trait;` |
| `covers(Foo::class)` | ✅ | `#[CoversClass(Foo::class)]` |
| `coversNothing()` | ✅ | `#[CoversNothing]` |
| `dataset('name', [...])` | ✅ | Static data provider method |
| `dataset('name', fn() => ...)` | ✅ | Generator-based provider |
| Describe-scoped `beforeEach`/`afterEach` | ✅ | Inlined into test methods (try/finally for afterEach) |
| Non-Pest code preserved | ✅ | Kept alongside generated class |

### Test Modifiers

| Modifier | Status | PHPUnit Output |
|---|:---:|---|
| `->skip('reason')` | ✅ | `$this->markTestSkipped(...)` |
| `->skip($condition, 'reason')` | ✅ | Conditional `if` + `markTestSkipped` |
| `->todo()` | ✅ | `$this->markTestIncomplete('TODO')` |
| `->group('name')` | ✅ | `#[Group('name')]` |
| `->depends('test')` | ✅ | `#[Depends('test_*')]` |
| `->covers(Foo::class)` | ✅ | `#[CoversClass(Foo::class)]` |
| `->with('dataset')` | ✅ | `#[DataProvider('dataset')]` |
| `->with([...])` | ✅ | Inline provider method + `#[DataProvider]` |
| Multiple `->with()` (cross-join) | ✅ | Composed cross-join provider method |
| `->throws(Exception::class)` | ✅ | `expectException` + `expectExceptionMessage` |
| `->after(fn() => ...)` | ✅ | Test body wrapped in `try/finally` |
| `->repeat(N)` | ✅ | `for` loop wrapping test body |
| `->only()` | ✅ | `#[Group('only')]` |

### `expect()` Assertions

#### Type Assertions

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toBeString` / `toBeInt` / `toBeFloat` / `toBeArray` | ✅ | `assertIsString` / `assertIsInt` / `assertIsFloat` / `assertIsArray` |
| `toBeBool` / `toBeCallable` / `toBeIterable` | ✅ | `assertIsBool` / `assertIsCallable` / `assertIsIterable` |
| `toBeNumeric` / `toBeObject` / `toBeResource` / `toBeScalar` | ✅ | `assertIsNumeric` / `assertIsObject` / `assertIsResource` / `assertIsScalar` |
| `toBeInstanceOf` | ✅ | `assertInstanceOf` |

#### Value Assertions

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toBe` | ✅ | `assertSame` |
| `toEqual` | ✅ | `assertEquals` |
| `toBeTrue` / `toBeFalse` | ✅ | `assertTrue` / `assertFalse` |
| `toBeTruthy` / `toBeFalsy` | ✅ | `assertNotEmpty` / `assertEmpty` |
| `toBeNull` | ✅ | `assertNull` |
| `toBeEmpty` | ✅ | `assertEmpty` |
| `toBeJson` | ✅ | `assertJson` |
| `toBeNan` / `toBeFinite` / `toBeInfinite` | ✅ | `assertNan` / `assertIsFinite` / `assertIsInfinite` |

#### Comparison Assertions

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toBeGreaterThan` / `toBeLessThan` | ✅ | `assertGreaterThan` / `assertLessThan` |
| `toBeGreaterThanOrEqual` / `toBeLessThanOrEqual` | ✅ | `assertGreaterThanOrEqual` / `assertLessThanOrEqual` |
| `toBeBetween($min, $max)` | ✅ | `assertGreaterThanOrEqual` + `assertLessThanOrEqual` |
| `toEqualWithDelta` | ✅ | `assertEqualsWithDelta` |
| `toEqualCanonicalizing` | ✅ | `assertEqualsCanonicalizing` |

#### String Assertions

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toStartWith` / `toEndWith` | ✅ | `assertStringStartsWith` / `assertStringEndsWith` |
| `toMatch` | ✅ | `assertMatchesRegularExpression` |
| `toContain` | ✅ | `assertContains` |
| `toContain($a, $b, $c)` (multi-arg) | ✅ | Multiple `assertContains` calls |

#### Array / Collection Assertions

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toHaveCount` / `toHaveLength` | ✅ | `assertCount` |
| `toHaveKey` | ✅ | `assertArrayHasKey` |
| `toHaveKeys(['a', 'b'])` | ✅ | Multiple `assertArrayHasKey` calls |
| `toContainEqual` | ✅ | `assertContainsEquals` |
| `toHaveSameSize` | ✅ | `assertSameSize` |
| `toBeList` | ✅ | `assertIsList` |
| `toMatchArray` / `toMatchObject` | ✅ | `assertEquals` |

#### Object Assertions

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toHaveProperty('name')` | ✅ | `assertObjectHasProperty` |
| `toHaveProperties(['a', 'b'])` | ✅ | Multiple `assertObjectHasProperty` calls |
| `toHaveProperties(['name' => 'John'])` | ✅ | `assertSame` per key-value pair |
| `toHaveMethod('foo')` | ✅ | `assertTrue(method_exists(...))` |
| `toMatchConstraint($c)` | ✅ | `assertThat($subject, $constraint)` |

#### File / Directory Assertions

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toBeFile` / `toBeDirectory` | ✅ | `assertFileExists` / `assertDirectoryExists` |
| `toBeReadableFile` / `toBeWritableFile` | ✅ | `assertFileIsReadable` / `assertFileIsWritable` |
| `toBeReadableDirectory` / `toBeWritableDirectory` | ✅ | `assertDirectoryIsReadable` / `assertDirectoryIsWritable` |

#### String Format Assertions (via regex)

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toBeUppercase` / `toBeLowercase` | ✅ | `assertMatchesRegularExpression` |
| `toBeAlpha` / `toBeAlphaNumeric` / `toBeDigits` | ✅ | `assertMatchesRegularExpression` |
| `toBeSnakeCase` / `toBeKebabCase` / `toBeCamelCase` / `toBeStudlyCase` | ✅ | `assertMatchesRegularExpression` |
| `toBeUuid` / `toBeUrl` | ✅ | `assertMatchesRegularExpression` |

#### Array Key Format Assertions

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toHaveSnakeCaseKeys` / `toHaveKebabCaseKeys` | ✅ | `foreach (array_keys(...))` + regex assert |
| `toHaveCamelCaseKeys` / `toHaveStudlyCaseKeys` | ✅ | `foreach (array_keys(...))` + regex assert |

#### Exception Assertions

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toThrow(Exception::class)` | ✅ | `expectException` + invoke callable |
| `toThrow(Exception::class, 'msg')` | ✅ | `expectException` + `expectExceptionMessage` |
| `toThrow(new Exception('msg'))` | ✅ | `expectException` + `expectExceptionMessage` |
| `toThrow('message')` | ✅ | `expectExceptionMessage` |
| `not->toThrow()` | ✅ | `try/catch` with `$this->fail()` on exception |

#### Laravel-Specific Assertions

| Pest Assertion | Status | PHPUnit Equivalent |
|---|:---:|---|
| `toBeCollection` | ✅ | `assertInstanceOf(Collection::class)` |
| `toBeModel` | ✅ | `assertInstanceOf(Model::class)` |
| `toBeEloquentCollection` | ✅ | `assertInstanceOf(EloquentCollection::class)` |

### Chain Modifiers

| Modifier | Status | Behavior |
|---|:---:|---|
| `->not->*` | ✅ | Negated equivalents (`assertNotSame`, `assertNotNull`, etc.) |
| `->and($subject)` | ✅ | Split into multiple assertion groups |
| `->each->*` (no closure) | ✅ | `foreach` loop with assertion per item |
| `->tap(fn() => ...)` | ✅ | Closure body inlined |
| `->pipe(fn($v) => ...)` | ✅ | Subject transformed: `(fn($v) => ...)($subject)` |
| Property access (e.g. `->name`) | ✅ | `$subject->name` |
| Method access (e.g. `->count()`) | ✅ | `$subject->count()` |

### Silently Stripped (debug/dev-only)

These Pest methods are removed from the chain without emitting any output:

| Modifier | Reason |
|---|---|
| `->dd()` / `->ddWhen()` / `->ddUnless()` | Debug — dump and die |
| `->ray()` | Debug — Ray debugger |
| `->json()` | Output modifier — no assertion equivalent |
| `->defer()` | Timing modifier — no assertion equivalent |

### Converted to `markTestSkipped` ⚠️

These features have no PHPUnit equivalent and are converted to skipped tests with a review comment:

| Pest Feature | PHPUnit Output |
|---|---|
| `arch()` tests | `$this->markTestSkipped('Arch test not supported in PHPUnit: ...')` |
| Higher-order `it('...')->expect([...])->toBeUsed()` | `$this->markTestSkipped('Arch test not supported in PHPUnit: ...')` |

### Emits `// TODO` Comment ⚠️

These features emit a TODO comment because they require manual conversion:

| Pest Feature | TODO Comment |
|---|---|
| `->sequence(...)` | `// TODO(Pest): ->sequence() requires manual conversion to PHPUnit` |
| `->match(...)` | `// TODO(Pest): ->match() requires manual conversion to PHPUnit` |
| `->scoped(...)` | `// TODO(Pest): ->scoped() requires manual conversion to PHPUnit` |
| `->each(fn() => ...)` (with closure) | `// TODO(Pest): ->each(closure) requires manual conversion to PHPUnit` |
| `->when(...)` / `->unless(...)` | `// TODO(Pest): ->when()/->unless() requires manual conversion to PHPUnit` |
| Unknown `->toXxx()` expectations | `// TODO(Pest): Unknown expectation ->toXxx() has no PHPUnit equivalent` |

### Laravel / Livewire `assert*()` Methods ✅

When `expect()` wraps a Laravel `TestResponse`, Livewire `Testable`, or any object with `assert*()` methods, they are emitted as **direct method calls** on the subject:

```php
// Before
expect($this->get('/'))->assertOk()->assertSee('Welcome');

// After
$this->get('/')->assertOk()->assertSee('Welcome');
```

This works automatically for **all** `assert*()` methods — no special mapping needed since these methods already throw PHPUnit assertions internally. Verified coverage includes:

**Laravel TestResponse:**

| Category | Methods |
|---|---|
| Status | `assertOk`, `assertCreated`, `assertNotFound`, `assertForbidden`, `assertUnauthorized`, `assertUnprocessable`, `assertStatus`, `assertSuccessful`, `assertNoContent` |
| Content | `assertSee`, `assertDontSee`, `assertSeeText`, `assertSeeInOrder`, `assertSeeTextInOrder` |
| JSON | `assertJson`, `assertExactJson`, `assertJsonFragment`, `assertJsonMissing`, `assertJsonStructure`, `assertJsonCount`, `assertJsonPath`, `assertJsonValidationErrors`, `assertJsonMissingValidationErrors` |
| Redirects | `assertRedirect`, `assertRedirectContains`, `assertRedirectToRoute`, `assertLocation` |
| Headers | `assertHeader`, `assertHeaderMissing` |
| Validation | `assertValid`, `assertInvalid`, `assertSessionHasErrors` |
| Session | `assertSessionHas`, `assertSessionHasAll`, `assertSessionMissing` |
| Views | `assertViewIs`, `assertViewHas`, `assertViewHasAll`, `assertViewMissing` |
| Cookies | `assertCookie`, `assertCookieMissing`, `assertCookieExpired` |
| Downloads | `assertDownload` |

**Livewire Testable:**

| Category | Methods |
|---|---|
| Content | `assertSee`, `assertDontSee`, `assertSeeHtml`, `assertDontSeeHtml`, `assertSeeInOrder` |
| Properties | `assertSet`, `assertNotSet`, `assertCount` |
| Events | `assertDispatched`, `assertNotDispatched` |
| Validation | `assertHasErrors`, `assertHasNoErrors` |
| Navigation | `assertRedirect`, `assertRedirectToRoute`, `assertNoRedirect` |
| Other | `assertStatus`, `assertForbidden`, `assertUnauthorized`, `assertViewHas`, `assertViewIs`, `assertFileDownloaded` |

Non-assert methods in the chain (like `followRedirects()`, `set()`, `call()`) are preserved naturally as chained method calls.

### Not Supported

| Pest Feature | Notes |
|---|---|
| `expect()->extend('name', fn)` | Custom expectation macros — emits TODO |
| Higher-order test methods (e.g. `it('...')->assertTrue()`) | Not converted |
| `beforeAll`/`afterAll` inside `describe()` | No clean PHPUnit equivalent without multiple classes |

## Limitations

- **Not a 100% migration tool.** Some Pest features have no direct PHPUnit equivalent — these are converted to skipped tests or TODO comments prompting manual review.
- **Assertion coverage** is broad (60+ mappings including negations) but doesn't cover every Pest plugin or custom expectation macro.
- **Method naming** uses `snake_case` with a `test_` prefix. Long `describe()` chains can produce long method names.
- **File structure** — the rule generates a class in-place. You may need to manually adjust namespaces or file locations.
- **String format assertions** (`toBeSnakeCase`, `toBeUuid`, etc.) use regex approximations that may not match Pest's exact validation logic.

## Development

```bash
git clone https://github.com/HelgeSverre/pest-to-phpunit.git
cd pest-to-phpunit
composer install

# Run tests
vendor/bin/phpunit
```

### Adding test fixtures

Test fixtures live in `tests/Rector/Fixture/` as `.php.inc` files with the format:

```
<?php
// Pest input code here

-----
<?php
// Expected PHPUnit output here
```

If the file should remain unchanged (no Pest code), omit the `-----` separator.

## License

MIT. See [LICENSE](LICENSE).

## Credits

Built by [Helge Sverre](https://helgesverre.com) on top of [Rector](https://getrector.com) and [nikic/php-parser](https://github.com/nikic/PHP-Parser).
