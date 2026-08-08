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
        if (is_string($__pest_contain_subject = $arr)) {
            $this->assertStringNotContainsString('x', $__pest_contain_subject);
        } else {
            $this->assertNotContains('x', $__pest_contain_subject);
        }
        $this->assertNotCount(5, $arr);
        $this->assertArrayNotHasKey('missing', $arr);
        $this->assertNotInstanceOf(Foo::class, $obj);
    }
}
