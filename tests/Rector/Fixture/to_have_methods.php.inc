<?php
test('has methods', function () {
    expect($obj)->toHaveMethods(['save', 'delete', 'update']);
});
-----
<?php
class ToHaveMethodsTest extends \PHPUnit\Framework\TestCase
{
    public function test_has_methods(): void
    {
        $this->assertTrue(method_exists($obj, 'save'));
        $this->assertTrue(method_exists($obj, 'delete'));
        $this->assertTrue(method_exists($obj, 'update'));
    }
}
