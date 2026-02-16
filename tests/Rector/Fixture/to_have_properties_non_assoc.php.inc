<?php
test('has properties', function () {
    $obj = new stdClass();
    $obj->name = 'test';
    $obj->age = 25;
    expect($obj)->toHaveProperties(['name', 'age']);
});

-----
<?php
class ToHavePropertiesNonAssocTest extends \PHPUnit\Framework\TestCase
{
    public function test_has_properties(): void
    {
        $obj = new stdClass();
        $obj->name = 'test';
        $obj->age = 25;
        $this->assertObjectHasProperty('name', $obj);
        $this->assertObjectHasProperty('age', $obj);
    }
}

