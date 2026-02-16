<?php
test('each with property', function () {
    expect($users)->each->toBeObject();
});

-----
<?php
class EachWithPropertyTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_with_property(): void
    {
        foreach ($users as $__each_item) {
            $this->assertIsObject($__each_item);
        }
    }
}
