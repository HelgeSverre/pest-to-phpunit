# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

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
- Line coverage: 91.91% (up from 90.56%)

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
