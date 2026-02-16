<?php
test('each greater than', function () {
    expect($numbers)->each->toBeGreaterThan(0);
});
-----
<?php
class EachToBeGreaterThanTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_greater_than(): void
    {
        foreach ($numbers as $__pest_each_item) {
            $this->assertGreaterThan(0, $__pest_each_item);
        }
    }
}
