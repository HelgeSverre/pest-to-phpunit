<?php
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

describe('math', function () {
    test('value is positive', function () {
        expect(5)->toBePositive();
    });
});

-----
<?php

class DescribeNestedWithCustomExpectationsTest extends \PHPUnit\Framework\TestCase
{
    public function test_math_value_is_positive(): void
    {
        $this->assertGreaterThan(0, 5);
    }
}
