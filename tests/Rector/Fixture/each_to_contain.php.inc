<?php
test('each toContain', function () {
    expect(['hello world', 'hello there'])->each->toContain('hello');
});
-----
<?php
class EachToContainTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_tocontain(): void
    {
        foreach (['hello world', 'hello there'] as $__pest_each_item) {
            if (is_string($__pest_each_item)) {
                $this->assertStringContainsString('hello', $__pest_each_item);
            } else {
                $this->assertContains('hello', $__pest_each_item);
            }
        }
    }
}
