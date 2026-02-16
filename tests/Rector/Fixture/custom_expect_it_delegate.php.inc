<?php
// Verifies custom expectations work with it() instead of test()
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

it('is positive', function () {
    expect(42)->toBePositive();
});

-----
<?php

class CustomExpectItDelegateTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_is_positive(): void
    {
        $this->assertGreaterThan(0, 42);
    }
}
