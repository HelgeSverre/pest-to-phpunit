<?php
test('each has key', function () {
    expect($items)->each->toHaveKey('id');
});
-----
<?php
class EachToHaveKeyTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_has_key(): void
    {
        foreach ($items as $__pest_each_item) {
            $this->assertArrayHasKey('id', $__pest_each_item);
        }
    }
}
