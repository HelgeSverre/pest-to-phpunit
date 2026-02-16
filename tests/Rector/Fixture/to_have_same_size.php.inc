<?php
test('same size', function () {
    expect([1, 2])->toHaveSameSize(['a', 'b']);
});

-----
<?php
class ToHaveSameSizeTest extends \PHPUnit\Framework\TestCase
{
    public function test_same_size(): void
    {
        $this->assertSameSize(['a', 'b'], [1, 2]);
    }
}
