<?php
test('does not have methods', function () {
    expect($obj)->not->toHaveMethods(['foo', 'bar']);
});
-----
<?php
class NotToHaveMethodsTest extends \PHPUnit\Framework\TestCase
{
    public function test_does_not_have_methods(): void
    {
        $this->assertFalse(method_exists($obj, 'foo'));
        $this->assertFalse(method_exists($obj, 'bar'));
    }
}
