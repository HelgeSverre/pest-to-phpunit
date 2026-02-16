<?php
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

expect()->extend('toBeEven', function () {
    expect($this->value % 2)->toBe(0);
});

test('positive and even', function () {
    expect(4)->toBePositive();
    expect(4)->toBeEven();
});

-----
<?php

class CustomExpectMultipleDefinitionsTest extends \PHPUnit\Framework\TestCase
{
    public function test_positive_and_even(): void
    {
        $this->assertGreaterThan(0, 4);
        $this->assertSame(0, 4 % 2);
    }
}
