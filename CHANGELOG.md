# Changelog

All notable changes to this project will be documented in this file.

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
