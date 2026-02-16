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
        foreach (['hello world', 'hello there'] as $__each_item) {
            $this->assertContains('hello', $__each_item);
        }
    }
}
