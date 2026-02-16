<?php
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

test('and chain with custom', function () {
    expect(5)->toBePositive()->and(10)->toBePositive();
});

-----
<?php

class CustomExpectAndChainTest extends \PHPUnit\Framework\TestCase
{
    public function test_and_chain_with_custom(): void
    {
        $this->assertGreaterThan(0, 5);
        $this->assertGreaterThan(0, 10);
    }
}
