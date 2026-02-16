<?php
test('special expectations', function () {
    expect($arr)->toMatchArray(['key' => 'value']);
    expect($obj)->toHaveProperty('name');
    expect($list)->toContainOnlyInstancesOf(User::class);
    expect($num)->toBeBetween(1, 10);
    expect($obj)->toHaveMethod('getName');
});

-----
<?php
class SpecialExpectsTest extends \PHPUnit\Framework\TestCase
{
    public function test_special_expectations(): void
    {
        $this->assertEquals(['key' => 'value'], $arr);
        $this->assertObjectHasProperty('name', $obj);
        $this->assertContainsOnlyInstancesOf(User::class, $list);
        $this->assertGreaterThanOrEqual(1, $num);
        $this->assertLessThanOrEqual(10, $num);
        $this->assertTrue(method_exists($obj, 'getName'));
    }
}
