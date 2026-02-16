<?php
test('equality checks', function () {
    expect($a)->toEqual($b);
    expect($a)->toBe($b);
    expect($a)->not->toEqual($b);
});

-----
<?php
class ToEqualTest extends \PHPUnit\Framework\TestCase
{
    public function test_equality_checks(): void
    {
        $this->assertEquals($b, $a);
        $this->assertSame($b, $a);
        $this->assertNotEquals($b, $a);
    }
}
