<?php
test('phpunit assertions directly', function () {
    $this->assertTrue(true);
    $this->assertFalse(false);
    $this->assertEquals(1, 1);
});
-----
<?php
class SingleTestNoExpectTest extends \PHPUnit\Framework\TestCase
{
    public function test_phpunit_assertions_directly(): void
    {
        $this->assertTrue(true);
        $this->assertFalse(false);
        $this->assertEquals(1, 1);
    }
}
