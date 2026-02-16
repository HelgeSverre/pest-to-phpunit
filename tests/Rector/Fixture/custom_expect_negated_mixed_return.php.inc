<?php
// Bug: negation was not passed through to expect() chains in mixed bodies
expect()->extend('toBeEven', function () {
    return expect($this->value % 2)->toBe(0);
});

test('negates even via return expect', function () {
    expect(5)->not->toBeEven();
});

-----
<?php

class CustomExpectNegatedMixedReturnTest extends \PHPUnit\Framework\TestCase
{
    public function test_negates_even_via_return_expect(): void
    {
        $this->assertNotSame(0, 5 % 2);
    }
}
