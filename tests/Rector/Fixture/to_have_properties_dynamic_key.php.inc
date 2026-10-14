<?php
test('dynamic property names', function () {
    $property = 'name';
    $object = (object) ['name' => 'Alice'];
    expect($object)->toHaveProperties([$property => 'Alice']);
});
-----
<?php
class ToHavePropertiesDynamicKeyTest extends \PHPUnit\Framework\TestCase
{
    public function test_dynamic_property_names(): void
    {
        $property = 'name';
        $object = (object) ['name' => 'Alice'];
        $this->assertSame('Alice', $object->{$property});
    }
}
