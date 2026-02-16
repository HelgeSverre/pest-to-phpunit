<?php
// Source: spatie/pest-expectations - toBeInRange
expect()->extend('toBeInRange', function (int $min, int $max) {
    $inRange = ($min <= $this->value) && ($this->value <= $max);
    expect($inRange)->toBeTrue();
});

test('value in range', function () {
    expect(50)->toBeInRange(1, 100);
});

-----
<?php

class CustomExpectThisValueTest extends \PHPUnit\Framework\TestCase
{
    public function test_value_in_range(): void
    {
        $inRange = (1 <= 50) && (50 <= 100);
        $this->assertTrue($inRange);
    }
}
