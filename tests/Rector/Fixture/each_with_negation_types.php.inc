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
        foreach ($items as $__each_item) {
            $this->assertNotEmpty($__each_item);
        }
        foreach ($items as $__each_item) {
            $this->assertNotNull($__each_item);
        }
    }
}
