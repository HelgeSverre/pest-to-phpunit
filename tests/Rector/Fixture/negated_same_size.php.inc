<?php
test('not same size', function () {
    expect([1, 2])->not->toHaveSameSize([1, 2, 3]);
});
-----
<?php
class NegatedSameSizeTest extends \PHPUnit\Framework\TestCase
{
    public function test_not_same_size(): void
    {
        $this->assertNotSameSize([1, 2, 3], [1, 2]);
    }
}
