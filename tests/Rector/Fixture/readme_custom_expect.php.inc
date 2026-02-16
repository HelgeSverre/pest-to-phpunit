<?php
expect()->extend('toBeWithinRange', function (int $min, int $max) {
    return $this->toBeGreaterThanOrEqual($min)
        ->toBeLessThanOrEqual($max);
});

test('value in range', function () {
    expect(100)->toBeWithinRange(90, 110);
});

-----
<?php

class ReadmeCustomExpectTest extends \PHPUnit\Framework\TestCase
{
    public function test_value_in_range(): void
    {
        $this->assertGreaterThanOrEqual(90, 100);
        $this->assertLessThanOrEqual(110, 100);
    }
}
