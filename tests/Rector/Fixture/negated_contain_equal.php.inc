<?php
test('not contain equal', function () {
    expect([["a" => 1]])->not->toContainEqual(["b" => 2]);
});
-----
<?php
class NegatedContainEqualTest extends \PHPUnit\Framework\TestCase
{
    public function test_not_contain_equal(): void
    {
        $this->assertNotContainsEquals(["b" => 2], [["a" => 1]]);
    }
}
