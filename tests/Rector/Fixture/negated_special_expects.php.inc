<?php
test('negated specials', function () {
    expect($arr)->not->toMatchArray(['key' => 'value']);
    expect($obj)->not->toHaveProperty('name');
    expect($num)->not->toBeBetween(1, 10);
    expect($obj)->not->toHaveMethod('getName');
});

-----
<?php
class NegatedSpecialExpectsTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_specials(): void
    {
        $this->assertNotEquals(['key' => 'value'], $arr);
        $this->assertObjectNotHasProperty('name', $obj);
        $this->assertLessThan(1, $num);
        $this->assertGreaterThan(10, $num);
        $this->assertFalse(method_exists($obj, 'getName'));
    }
}
