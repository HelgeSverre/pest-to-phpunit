<?php
test('has multiple properties', function () {
    expect($obj)->toHaveProperty('name');
    expect($obj)->toHaveProperty('age');
});
-----
<?php
class ToHavePropertiesPluralTest extends \PHPUnit\Framework\TestCase
{
    public function test_has_multiple_properties(): void
    {
        $this->assertObjectHasProperty('name', $obj);
        $this->assertObjectHasProperty('age', $obj);
    }
}
