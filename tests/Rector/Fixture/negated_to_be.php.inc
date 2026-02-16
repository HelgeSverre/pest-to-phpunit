<?php
test('negated toBe', function () {
    expect($x)->not->toBe(5);
    expect($y)->not->toEqual('foo');
});
-----
<?php
class NegatedToBeTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_tobe(): void
    {
        $this->assertNotSame(5, $x);
        $this->assertNotEquals('foo', $y);
    }
}
