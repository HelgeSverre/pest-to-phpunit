<?php
// Bug: negation was not passed through to expect() expression chains in mixed bodies
expect()->extend('toBeEven', function () {
    expect($this->value % 2)->toBe(0);
});

test('negates even via expect expression', function () {
    expect(5)->not->toBeEven();
});

-----
<?php

class CustomExpectNegatedMixedExprTest extends \PHPUnit\Framework\TestCase
{
    public function test_negates_even_via_expect_expression(): void
    {
        $this->assertNotSame(0, 5 % 2);
    }
}
