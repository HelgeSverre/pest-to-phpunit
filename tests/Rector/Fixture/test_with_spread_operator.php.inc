<?php
test('spread operator', function () {
    $args = [1, 2, 3];
    $result = array_sum([...$args, 4]);
    expect($result)->toBe(10);
});
-----
<?php
class TestWithSpreadOperatorTest extends \PHPUnit\Framework\TestCase
{
    public function test_spread_operator(): void
    {
        $args = [1, 2, 3];
        $result = array_sum([...$args, 4]);
        $this->assertSame(10, $result);
    }
}
