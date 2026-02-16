<?php
test('each not type', function () {
    expect($items)->each->not->toBeEmpty();
    expect($items)->each->not->toBeNull();
});
-----
<?php
class EachWithNegationTypesTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_not_type(): void
    {
        foreach ($items as $__pest_each_item) {
            $this->assertNotEmpty($__pest_each_item);
        }
        foreach ($items as $__pest_each_item) {
            $this->assertNotNull($__pest_each_item);
        }
    }
}
