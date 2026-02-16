<?php
test('not equal with delta', function () {
    expect(1.0)->not->toEqualWithDelta(5.0, 0.1);
});
-----
<?php
class NegatedEqualWithDeltaTest extends \PHPUnit\Framework\TestCase
{
    public function test_not_equal_with_delta(): void
    {
        $this->assertNotEqualsWithDelta(5.0, 1.0, 0.1);
    }
}
