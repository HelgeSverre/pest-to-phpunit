# Pest to PHPUnit — Rector Extension

[![Latest Version on Packagist](https://img.shields.io/packagist/v/helgesverre/pest-to-phpunit.svg?style=flat-square)](https://packagist.org/packages/helgesverre/pest-to-phpunit)
[![Total Downloads](https://img.shields.io/packagist/dt/helgesverre/pest-to-phpunit.svg?style=flat-square)](https://packagist.org/packages/helgesverre/pest-to-phpunit)
[![PHP Version](https://img.shields.io/packagist/php-v/helgesverre/pest-to-phpunit.svg?style=flat-square)](https://packagist.org/packages/helgesverre/pest-to-phpunit)
[![License](https://img.shields.io/packagist/l/helgesverre/pest-to-phpunit.svg?style=flat-square)](LICENSE)

A **Rector extension** that automatically converts **Pest** test files into **PHPUnit** test classes.

Handles `test()` / `it()` blocks, hooks, datasets, `expect()` assertion chains, modifiers, and more — getting you most of the way there automatically while leaving clear markers for anything that needs manual review.

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

### Core constructs

| Pest feature | Status | PHPUnit output |
|---|:---:|---|
| `test()` / `it()` | ✅ | `public function test_*(): void` |
| `describe()` (nested) | ✅ | Method name prefixing |
| `beforeEach` / `afterEach` | ✅ | `setUp()` / `tearDown()` |
| `beforeAll` / `afterAll` | ✅ | `setUpBeforeClass()` / `tearDownAfterClass()` |
| `uses()` | ✅ | `extends` + `use` traits |
| `covers()` | ✅ | `#[CoversClass]` |
| `coversNothing()` | ✅ | `#[CoversNothing]` |
| Non-Pest code preserved | ✅ | Kept alongside generated class |

### `expect()` assertion mapping

| Pest assertion | PHPUnit equivalent |
|---|---|
| `toBe` | `assertSame` |
| `toEqual` | `assertEquals` |
| `toBeTrue` / `toBeFalse` | `assertTrue` / `assertFalse` |
| `toBeNull` | `assertNull` |
| `toBeEmpty` | `assertEmpty` |
| `toContain` | `assertContains` |
| `toHaveCount` / `toHaveLength` | `assertCount` |
| `toBeInstanceOf` | `assertInstanceOf` |
| `toMatch` | `assertMatchesRegularExpression` |
| `toStartWith` / `toEndWith` | `assertStringStartsWith` / `assertStringEndsWith` |
| `toBeString` / `toBeInt` / `toBeFloat` / `toBeArray` / `toBeBool` | `assertIs*` |
| `toBeGreaterThan` / `toBeLessThan` (and `OrEqual` variants) | `assertGreaterThan` / `assertLessThan` |
| `toHaveKey` | `assertArrayHasKey` |
| `toBeFile` / `toBeDirectory` | `assertFileExists` / `assertDirectoryExists` |
| `toBeJson` | `assertJson` |
| `toBeNan` / `toBeFinite` / `toBeInfinite` | `assertNan` / `assertIsFinite` / `assertIsInfinite` |
| `toThrow` | `expectException` + invoke callable |
| `toMatchArray` | `assertEquals` |
| `toHaveProperty` | `assertObjectHasProperty` |
| `toBeBetween` | `assertGreaterThanOrEqual` + `assertLessThanOrEqual` |
| `toHaveMethod` | `assertTrue(method_exists(...))` |
| `->not->*` | Negated equivalents (`assertNotSame`, `assertNotNull`, etc.) |
| `->and(...)` | Split into multiple assertions |
| `->each->*` | `foreach` loop with assertion |

### Test modifiers

| Modifier | PHPUnit output |
|---|---|
| `->skip('reason')` | `$this->markTestSkipped(...)` |
| `->todo()` | `$this->markTestIncomplete('TODO')` |
| `->group('name')` | `#[Group('name')]` |
| `->depends('test')` | `#[Depends('test_*')]` |
| `->covers(Foo::class)` | `#[CoversClass(Foo::class)]` |
| `->with('dataset')` | `#[DataProvider('dataset')]` |
| `->with([...])` | Inline provider method + `#[DataProvider]` |
| `->throws(Exception::class)` | `expectException` + `expectExceptionMessage` |

### Partial / manual review needed

| Pest feature | Output |
|---|---|
| `arch()` tests | `markTestSkipped` with review comment |
| Higher-order expectations (`it()->expect()->...`) | `markTestSkipped` with review comment |
| `sequence`, `json`, `defer`, `ray` modifiers | Silently skipped |

## Limitations

- **Not a 100% migration tool.** Some Pest features have no direct PHPUnit equivalent — these are converted to skipped tests with a docblock prompting manual review.
- **Assertion coverage** is broad (~40 mappings) but doesn't cover every Pest plugin or custom macro.
- **Method naming** uses `snake_case` with a `test_` prefix. Long `describe()` chains can produce long method names.
- **File structure** — the rule generates a class in-place. You may need to manually adjust namespaces or file locations.

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
