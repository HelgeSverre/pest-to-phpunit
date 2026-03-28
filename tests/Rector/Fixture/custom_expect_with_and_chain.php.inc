<?php
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

test('positive and string', function () {
    expect(5)->toBePositive()->and('hello')->toBeString();
});

-----
<?php

class CustomExpectWithAndChainTest extends \PHPUnit\Framework\TestCase
{
    public function test_positive_and_string(): void
    {
        $this->assertGreaterThan(0, 5);
        $this->assertIsString('hello');
    }
}
