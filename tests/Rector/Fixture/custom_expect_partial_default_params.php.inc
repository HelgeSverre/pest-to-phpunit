<?php
// Edge case: first param provided, second uses default
expect()->extend('toBeInDefaultRange', function ($min, $max = 10) {
    return $this
        ->toBeGreaterThanOrEqual($min)
        ->toBeLessThanOrEqual($max);
});

test('uses default max', function () {
    expect(5)->toBeInDefaultRange(3);
});

-----
<?php

class CustomExpectPartialDefaultParamsTest extends \PHPUnit\Framework\TestCase
{
    public function test_uses_default_max(): void
    {
        $this->assertGreaterThanOrEqual(3, 5);
        $this->assertLessThanOrEqual(10, 5);
    }
}
