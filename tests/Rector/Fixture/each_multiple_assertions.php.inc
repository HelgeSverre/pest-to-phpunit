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
        foreach ($items as $__pest_each_item) {
            $this->assertIsString($__pest_each_item);
        }
        foreach ($items as $__pest_each_item) {
            if (is_string($__pest_each_item)) {
                $this->assertStringContainsString('test', $__pest_each_item);
            } else {
                $this->assertContains('test', $__pest_each_item);
            }
        }
    }
}
