<?php
expect()->extend('toBePositive', function () {
    $this->toBeGreaterThan(0);
});

expect()->extend('toBeEven', function () {
    expect($this->value % 2)->toBe(0);
});

test('custom expects', function () {
    expect(5)->toBePositive();
    expect(4)->toBeEven();
});
-----
<?php

class MultipleExpectExtendTest extends \PHPUnit\Framework\TestCase
{
    public function test_custom_expects(): void
    {
        $this->assertGreaterThan(0, 5);
        $this->assertSame(0, 4 % 2);
    }
}
