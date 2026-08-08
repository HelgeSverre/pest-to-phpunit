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
        $this->assertTrue(5 < 1 || 5 > 10);
    }
}
