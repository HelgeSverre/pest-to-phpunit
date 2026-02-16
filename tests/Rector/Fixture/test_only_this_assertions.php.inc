<?php
test('this assertions only', function () {
    $this->assertTrue(true);
    $this->assertSame(1, 1);
    $this->assertCount(3, [1, 2, 3]);
    $this->assertStringContainsString('hello', 'hello world');
});
-----
<?php
class TestOnlyThisAssertionsTest extends \PHPUnit\Framework\TestCase
{
    public function test_this_assertions_only(): void
    {
        $this->assertTrue(true);
        $this->assertSame(1, 1);
        $this->assertCount(3, [1, 2, 3]);
        $this->assertStringContainsString('hello', 'hello world');
    }
}
