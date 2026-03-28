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
        $this->assertLessThan(1, [0, 11, 20]);
        $this->assertGreaterThan(10, [0, 11, 20]);
    }
}
