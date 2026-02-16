<?php
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

test('all values are positive', function (int $value) {
    expect($value)->toBePositive();
})->with([1, 5, 10, 100]);

-----
<?php

class CustomExpectWithDatasetTest extends \PHPUnit\Framework\TestCase
{
    public static function test_all_values_are_positive_provider(): array
    {
        return [[1], [5], [10], [100]];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('test_all_values_are_positive_provider')]
    public function test_all_values_are_positive(int $value): void
    {
        $this->assertGreaterThan(0, $value);
    }
}
