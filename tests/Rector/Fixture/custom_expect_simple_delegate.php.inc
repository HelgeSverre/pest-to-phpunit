<?php
// Source: pestphp/pest stubs/init/Pest.php.stub
expect()->extend('toBeOne', function () {
    return $this->toBe(1);
});

test('value is one', function () {
    expect(1)->toBeOne();
});

-----
<?php

class CustomExpectSimpleDelegateTest extends \PHPUnit\Framework\TestCase
{
    public function test_value_is_one(): void
    {
        $this->assertSame(1, 1);
    }
}
