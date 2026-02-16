<?php
// Source: pestphp/pest docs - Custom Expectations
expect()->extend('toBeWithinRange', function (int $min, int $max) {
    return $this->toBeGreaterThanOrEqual($min)
        ->toBeLessThanOrEqual($max);
});

test('numeric ranges', function () {
    expect(100)->toBeWithinRange(90, 110);
});

-----
<?php

class CustomExpectWithParamsTest extends \PHPUnit\Framework\TestCase
{
    public function test_numeric_ranges(): void
    {
        $this->assertGreaterThanOrEqual(90, 100);
        $this->assertLessThanOrEqual(110, 100);
    }
}
