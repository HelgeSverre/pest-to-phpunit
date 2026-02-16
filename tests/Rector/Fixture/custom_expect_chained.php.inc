<?php
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

test('chained with custom', function () {
    expect(42)->toBeInt()->toBePositive()->toBeLessThan(100);
});

-----
<?php

class CustomExpectChainedTest extends \PHPUnit\Framework\TestCase
{
    public function test_chained_with_custom(): void
    {
        $this->assertIsInt(42);
        $this->assertGreaterThan(0, 42);
        $this->assertLessThan(100, 42);
    }
}
