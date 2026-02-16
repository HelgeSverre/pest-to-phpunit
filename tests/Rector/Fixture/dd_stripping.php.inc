<?php
test('strips debug methods', function () {
    expect($value)->dd()->toBe(42);
});

-----
<?php
class DdStrippingTest extends \PHPUnit\Framework\TestCase
{
    public function test_strips_debug_methods(): void
    {
        $this->assertSame(42, $value);
    }
}
