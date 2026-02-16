<?php
// Edge case: closure params have default values, no args passed at call site
expect()->extend('toBeInDefaultRange', function ($min = 0, $max = 10) {
    return $this
        ->toBeGreaterThanOrEqual($min)
        ->toBeLessThanOrEqual($max);
});

test('uses defaults', function () {
    expect(5)->toBeInDefaultRange();
});

-----
<?php

class CustomExpectDefaultParamsTest extends \PHPUnit\Framework\TestCase
{
    public function test_uses_defaults(): void
    {
        $this->assertGreaterThanOrEqual(0, 5);
        $this->assertLessThanOrEqual(10, 5);
    }
}
