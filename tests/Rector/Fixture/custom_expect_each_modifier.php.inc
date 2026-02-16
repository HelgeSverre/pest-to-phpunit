<?php
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

test('each item is positive', function () {
    expect([1, 2, 3])->each->toBePositive();
});

-----
<?php

class CustomExpectEachModifierTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_item_is_positive(): void
    {
        foreach ([1, 2, 3] as $__pest_each_item) {
            $this->assertGreaterThan(0, $__pest_each_item);
        }
    }
}
