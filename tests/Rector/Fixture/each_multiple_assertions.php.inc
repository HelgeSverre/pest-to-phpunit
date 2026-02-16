<?php
test('each multiple', function () {
    expect($items)->each->toBeString()->toContain('test');
});
-----
<?php
class EachMultipleAssertionsTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_multiple(): void
    {
        foreach ($items as $__each_item) {
            $this->assertIsString($__each_item);
        }
        foreach ($items as $__each_item) {
            $this->assertContains('test', $__each_item);
        }
    }
}
