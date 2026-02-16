<?php
// Source: spatie/pest-expectations
expect()->extend('toBeInRange', function (int $min, int $max) {
    return $this->toBeGreaterThanOrEqual($min)
        ->toBeLessThanOrEqual($max);
});

expect()->extend('toBeCloseTo', function (float $expected, float $deviation) {
    $minimum = $expected - $deviation;
    $maximum = $expected + $deviation;
    return $this->toBeInRange($minimum, $maximum);
});

test('close to value', function () {
    expect(10.2)->toBeCloseTo(10.0, 0.5);
});

-----
<?php

class CustomExpectCompositionTest extends \PHPUnit\Framework\TestCase
{
    public function test_close_to_value(): void
    {
        $minimum = 10.0 - 0.5;
        $maximum = 10.0 + 0.5;
        $this->assertGreaterThanOrEqual($minimum, 10.2);
        $this->assertLessThanOrEqual($maximum, 10.2);
    }
}
