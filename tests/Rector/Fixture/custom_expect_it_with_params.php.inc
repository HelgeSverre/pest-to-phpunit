<?php
// Verifies custom expectations with params work with it()
expect()->extend('toBeWithinRange', function (int $min, int $max) {
    return $this->toBeGreaterThanOrEqual($min)
        ->toBeLessThanOrEqual($max);
});

it('falls within range', function () {
    expect(50)->toBeWithinRange(1, 100);
});

-----
<?php

class CustomExpectItWithParamsTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_falls_within_range(): void
    {
        $this->assertGreaterThanOrEqual(1, 50);
        $this->assertLessThanOrEqual(100, 50);
    }
}
