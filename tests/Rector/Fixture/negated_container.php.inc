<?php
test('negated container', function () {
    expect($arr)->not->toContain('x');
    expect($arr)->not->toHaveCount(5);
    expect($arr)->not->toHaveKey('missing');
    expect($obj)->not->toBeInstanceOf(Foo::class);
});
-----
<?php
class NegatedContainerTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_container(): void
    {
        $this->assertNotContains('x', $arr);
        $this->assertNotCount(5, $arr);
        $this->assertArrayNotHasKey('missing', $arr);
        $this->assertNotInstanceOf(Foo::class, $obj);
    }
}
