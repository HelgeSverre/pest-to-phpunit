<?php
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

test('each item is not positive', function () {
    expect([-1, -2, -3])->each->not->toBePositive();
});

-----
<?php

class EachNotCustomExpectationTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_item_is_not_positive(): void
    {
        foreach ([-1, -2, -3] as $__pest_each_item) {
            $this->assertLessThanOrEqual(0, $__pest_each_item);
        }
    }
}
