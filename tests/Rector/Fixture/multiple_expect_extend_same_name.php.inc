<?php
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThanOrEqual(1);
});

test('uses latest definition', function () {
    expect(5)->toBePositive();
});

-----
<?php

class MultipleExpectExtendSameNameTest extends \PHPUnit\Framework\TestCase
{
    public function test_uses_latest_definition(): void
    {
        $this->assertGreaterThanOrEqual(1, 5);
    }
}
