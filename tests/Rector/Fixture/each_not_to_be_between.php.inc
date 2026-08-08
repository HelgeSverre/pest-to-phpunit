<?php
test('each not between', function () {
    expect([0, 11, 20])->each->not->toBeBetween(1, 10);
});
-----
<?php
class EachNotToBeBetweenTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_not_between(): void
    {
        foreach ([0, 11, 20] as $__pest_each_item) {
            $this->assertTrue($__pest_each_item < 1 || $__pest_each_item > 10);
        }
    }
}
