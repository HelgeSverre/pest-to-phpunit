<?php
expect()->extend('toBePositive', function () {
    $this->toBeGreaterThan(0);
});

expect()->extend('toBeEven', function () {
    expect($this->value % 2)->toBe(0);
});

test('custom expects', function () {
    expect(5)->toBePositive();
    expect(4)->toBeEven();
});
-----
<?php

/**
 * TODO(Pest): The following custom expectations were defined via expect()->extend()
 * and cannot be auto-converted to PHPUnit:
 *   - toBePositive
 *   - toBeEven
 */
class MultipleExpectExtendTest extends \PHPUnit\Framework\TestCase
{
    // TODO(Pest): Custom expectation 'toBePositive' defined via expect()->extend() cannot be auto-converted to PHPUnit
    // TODO(Pest): Custom expectation 'toBeEven' defined via expect()->extend() cannot be auto-converted to PHPUnit
    public function test_custom_expects(): void
    {
        // TODO(Pest): Unknown expectation ->toBePositive() has no PHPUnit equivalent
        // TODO(Pest): Unknown expectation ->toBeEven() has no PHPUnit equivalent
    }
}
