<?php
test('each toBe', function () {
    expect([1, 1, 1])->each->toBe(1);
});

-----
<?php
class EachToBeTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_tobe(): void
    {
        foreach ([1, 1, 1] as $__pest_each_item) {
            $this->assertSame(1, $__pest_each_item);
        }
    }
}
