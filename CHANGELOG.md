# Changelog

All notable changes to this project will be documented in this file.

## [v0.0.7] - 2026-02-17

### New Features
- **Namespace inference from `composer.json` PSR-4 mappings** — When converting Pest files that have no `namespace` declaration (the common case), the tool can now automatically infer the correct namespace from `composer.json` `autoload-dev` / `autoload` PSR-4 mappings. This prevents class name collisions (e.g. `tests/Unit/ExampleTest.php` and `tests/Feature/ExampleTest.php` both producing `ExampleTest`). Enable via `withConfiguredRule`:
  ```php
  use HelgeSverre\PestToPhpUnit\Rector\PestFileToPhpUnitClassRector;

  return RectorConfig::configure()
      ->withConfiguredRule(PestFileToPhpUnitClassRector::class, [
          PestFileToPhpUnitClassRector::INFER_NAMESPACE => true,
      ]);
  ```
- **Configurable rule** — `PestFileToPhpUnitClassRector` now implements `ConfigurableRectorInterface`, allowing configuration via `withConfiguredRule()` in `rector.php`

### Bug Fixes
- **Data provider methods treated as test methods** — Provider methods were named `test_XXX_provider()` which PHPUnit also interpreted as test methods, causing "risky test" warnings. Provider names now strip the `test_` prefix (e.g. `it_adds_correctly_provider()`)

### Testing
- **353 tests, 3,600 assertions** — all passing
- Verified end-to-end on a real Laravel project with Pest: 45 Pest tests converted to PHPUnit with zero failures

## [v0.0.6] - 2026-02-16

### Bug Fixes
- **Negation not propagated to `expect()` chains in mixed custom expectation bodies** — When a custom expectation used `expect($this->value)->toBe()` (rather than `$this->toBe()`), call-site `->not->` negation was silently dropped, producing incorrect assertions
- **Delegate body only inlined first statement** — Custom expectations with multiple `$this->toXxx()` statements (e.g. `$this->toBeString(); $this->toContain('@');`) only emitted the first assertion, silently dropping the rest
- **`$this->value->method()` misclassified as delegation** — Expressions like `$this->value->assertOk()` were wrongly classified as `$this->toXxx()` delegation chains, producing empty output instead of routing through the complex body handler
- **`not` modifier didn't toggle** — `$negated = true` instead of `$negated = !$negated` in the chain unwinder, breaking double-negation scenarios (e.g. `->not->toBeNotNullCustom()`)
- **Unknown `return` expressions silently dropped** — Custom expectation bodies with `return (bool) $this->value` or similar non-chain returns were misclassified as `delegate`, producing empty output instead of routing through the complex body handler with a TODO comment
- **PHPUnit config scanning fixture files** — `phpunit.xml` scanned all of `tests/` recursively, causing CI failures when PHPUnit tried to load Pest fixture files (e.g. `ExtractorManagerTest.php`) as test classes

### Testing
- **352 tests, 3,590 assertions** (up from 338 / 3,450)
- 14 new fixtures covering: negated mixed bodies (return + expression), delegate multi-statement, default params (full + partial), extra args, `$this->value` with array access and property fetch, complex method-on-value (Laravel style), double negation, return-non-chain classification, `it()` variants (delegate, mixed, params)

### CI
- Added PHP 8.5 to test matrix (now 8.1–8.5)
- Fixed `phpunit.xml` to exclude `tests/Rector/Fixture` directory

## [v0.0.5] - 2026-02-16

### New Features
- **Custom expectation inlining** — `expect()->extend('name', fn)` definitions are now parsed, registered, and inlined at call sites instead of emitting TODO comments. Supports delegating bodies (`$this->toBeGreaterThan(0)`), mixed bodies with `expect()` chains and local variables, `$this->value` substitution, closure parameter mapping, and arrow function bodies. Complex bodies that can't be fully inlined get best-effort conversion with a TODO comment.
- **Pest Faker plugin support** — `fake()` and `Pest\Faker\fake()` calls are converted to `\Faker\Factory::create()`, with locale arguments preserved. `use function Pest\Faker\fake;` imports are automatically stripped. `$this->faker` via `WithFaker` trait already worked via trait conversion.
- **Pest function import stripping** — All `use function Pest\...` imports (Faker, Laravel, Livewire) are now automatically removed during conversion, including grouped imports like `use function Pest\Laravel\{get, post};`
- **Nested `expect()` chain conversion** — `expect()` chains inside `if`, `else`, `elseif`, `foreach`, `for`, `while`, `try`, `catch`, `finally`, and `switch/case` blocks are now correctly converted to PHPUnit assertions. Previously these were silently dropped, producing empty control structure blocks.

### Bug Fixes
- **`expect()` inside control structures silently dropped** — Fixed `transformBody()` to recursively process nested statement blocks. This affected `if`, `foreach`, `for`, `while`, `try/catch/finally`, and `switch/case` blocks. 4 existing fixtures had incorrect expected output due to this bug and were corrected.
- **`toContain` now uses `assertStringContainsString`** for string literal subjects instead of always using `assertContains`, which is for arrays/iterables.
- **Inline dataset wrapping** — Single-value datasets like `['red', 'green', 'blue']` are now correctly wrapped as `[['red'], ['green'], ['blue']]` for PHPUnit data providers.

### Testing
- **338 tests, 3,450 assertions** (up from 399 tests / 3,390 assertions in v0.0.4)
- 15 new custom expectation fixtures: `custom_expect_simple_delegate`, `custom_expect_chained`, `custom_expect_with_params`, `custom_expect_this_value`, `custom_expect_negated`, `custom_expect_arrow_function`, `custom_expect_multiple_assertions`, `custom_expect_multiple_definitions`, `custom_expect_and_chain`, `custom_expect_composition`, `custom_expect_complex_body`, `custom_expect_each_modifier`, `custom_expect_file_extension`, `custom_expect_with_dataset`, `custom_expect_definition_only`
- 12 new fixtures: `faker_plugin`, `faker_advanced`, `nested_expect`, `nested_expect_elseif`, `nested_expect_switch`, `nested_expect_finally`, `not_to_throw_no_class`, `after_arrow_function`, `describe_arrow_function_hooks`, `to_have_properties_non_assoc`, `to_throw_class_with_message`, `readme_faker`
- 4 existing fixtures corrected (`test_early_return`, `test_with_foreach`, `test_with_if_statement`, `test_with_try_catch`)

## [v0.0.4] - 2026-02-16

### New Features
- **Laravel/Livewire `assert*()` chain support** — `expect($this->get('/'))->assertOk()->assertSee('Welcome')` converts to direct method calls on the subject, with chained asserts preserved naturally
- **GitHub Actions CI** — PHPUnit test matrix across PHP 8.1, 8.2, 8.3, and 8.4

### Bug Fixes
- **`tap()` with ArrowFunction** — Previously silently dropped; now properly inlines the expression
- **`tap()` with closure parameters** — `->tap(fn($v) => $v->toBeInt())` now assigns the subject to the parameter variable and unwraps nested expect chains
- **`toThrow('An Error')` misclassification** — Strings like "An Error" ending with "Error" were incorrectly classified as exception class names; now requires valid identifier format
- **`toHaveLength()` for strings** — Now uses `is_string($x) ? strlen($x) : count($x)` ternary instead of always using `assertCount`
- **`not->toBeJson()` negation** — Removed incorrect `assertJson → assertIsNotString` fallback from negation map
- **`toBeUppercase`/`toBeLowercase` regex** — Fixed to use `\P{Ll}` / `\P{Lu}` Unicode property escapes for accuracy
- **`each->toHaveLength()` regression** — Each-mode now correctly emits foreach with per-item length check

### Improvements
- **Collision-proof variable naming** — Internal `$__each_item` renamed to `$__pest_each_item` and extracted to class constant
- **README** — Added step 3 "Review TODO comments" with `grep -rn "TODO(Pest)"` guidance

### Testing
- **399 tests, 3,390 assertions** (up from 290 tests in v0.0.3)
- Added metamorphic invariant assertions on all fixture tests (no bare `expect()`, `test()`, `it()` calls survive in output)
- Added `ExpectChainUnwinder` unit test matrix: toThrow string classification (16 cases), modifier combos (10 cases), tap callable shapes (4 tests), exhaustive MAP negation (44 cases), each-mode coverage (8 cases)
- 12 new Laravel/Livewire test fixtures covering TestResponse, Livewire Testable, JSON testing, redirects, validation, views, cookies, and mixed chains
- 8 new correctness fixtures (tap variants, toThrow variants, each+toHaveLength, not->toBeJson, describe beforeAll TODO)

## [v0.0.3] - 2026-02-16

### New Features
- Higher-order arch tests properly convert to skipped tests
- `isPestExpression()` and `unwrapChain()` traverse PropertyFetch nodes

### New Test Fixtures
- 15 new fixtures: `toMatchConstraint`, multi-arg `toContain`, `toHaveXxxCaseKeys`, debug method stripping, TODO modifiers, `->after()` hook, higher-order arch tests, README verification fixtures

### Bug Fixes
- Fixed 3 fixtures that left unconverted Pest code in expected output

### Documentation
- Comprehensive README with 60+ assertion mappings in categorized tables

**290 tests, 670 assertions**

## [v0.0.2] - 2026-02-16

### Features
- Describe-scoped hooks (beforeEach/afterEach inlined with try/finally)
- `not->toThrow()` with try/catch assertions
- Multiple `->with()` cross-join datasets
- `->pipe(callable)` subject transformation
- 50+ Pest expectations mapped
- PHP syntax validation on all fixture outputs

**269 tests, 610 assertions**

## [v0.0.1] - 2026-02-16

- Initial release with basic Pest-to-PHPUnit conversion
