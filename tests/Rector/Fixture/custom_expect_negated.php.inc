<?php
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

test('negated custom expectation', function () {
    expect(-5)->not->toBePositive();
});

-----
<?php

class CustomExpectNegatedTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_custom_expectation(): void
    {
        $this->assertLessThanOrEqual(0, -5);
    }
}
