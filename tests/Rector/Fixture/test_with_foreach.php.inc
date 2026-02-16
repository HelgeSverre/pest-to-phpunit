<?php
test('iterates items', function () {
    $items = ['a', 'b', 'c'];
    foreach ($items as $item) {
        expect($item)->toBeString();
    }
});
-----
<?php
class TestWithForeachTest extends \PHPUnit\Framework\TestCase
{
    public function test_iterates_items(): void
    {
        $items = ['a', 'b', 'c'];
        foreach ($items as $item) {
            $this->assertIsString($item);
        }
    }
}
