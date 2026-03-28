<?php
test('not between', function () {
    expect(5)->not->toBeBetween(1, 10);
});

-----
<?php
class NotToBeBetweenTest extends \PHPUnit\Framework\TestCase
{
    public function test_not_between(): void
    {
        $this->assertLessThan(1, 5);
        $this->assertGreaterThan(10, 5);
    }
}
