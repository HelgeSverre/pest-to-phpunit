<?php
// Edge case: extra call-site args beyond what the closure accepts should be ignored
expect()->extend('toBeBetween', function ($min, $max) {
    return $this->toBeGreaterThanOrEqual($min)->toBeLessThanOrEqual($max);
});

test('extra args are ignored', function () {
    expect(5)->toBeBetween(1, 10, 999);
});

-----
<?php

class CustomExpectExtraArgsTest extends \PHPUnit\Framework\TestCase
{
    public function test_extra_args_are_ignored(): void
    {
        $this->assertGreaterThanOrEqual(1, 5);
        $this->assertLessThanOrEqual(10, 5);
    }
}
