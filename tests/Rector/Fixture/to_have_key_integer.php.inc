<?php
test('integer key', function () {
    expect(['a', 'b', 'c'])->toHaveKey(0);
    expect(['a', 'b', 'c'])->toHaveKey(2);
    expect(['a', 'b', 'c'])->not->toHaveKey(5);
});
-----
<?php
class ToHaveKeyIntegerTest extends \PHPUnit\Framework\TestCase
{
    public function test_integer_key(): void
    {
        $this->assertArrayHasKey(0, ['a', 'b', 'c']);
        $this->assertArrayHasKey(2, ['a', 'b', 'c']);
        $this->assertArrayNotHasKey(5, ['a', 'b', 'c']);
    }
}
