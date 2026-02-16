<?php
test('contains multiple', function () {
    expect($items)->toContain('a', 'b', 'c');
});
-----
<?php
class MultiArgToContainTest extends \PHPUnit\Framework\TestCase
{
    public function test_contains_multiple(): void
    {
        $this->assertContains('a', $items);
        $this->assertContains('b', $items);
        $this->assertContains('c', $items);
    }
}
