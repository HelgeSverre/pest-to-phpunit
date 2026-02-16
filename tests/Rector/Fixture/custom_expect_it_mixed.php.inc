<?php
// Verifies mixed custom expectation with it() and negation
expect()->extend('toBeEven', function () {
    expect($this->value % 2)->toBe(0);
});

it('rejects odd numbers', function () {
    expect(5)->not->toBeEven();
});

-----
<?php

class CustomExpectItMixedTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_rejects_odd_numbers(): void
    {
        $this->assertNotSame(0, 5 % 2);
    }
}
