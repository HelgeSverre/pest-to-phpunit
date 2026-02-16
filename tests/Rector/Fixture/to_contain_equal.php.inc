<?php
test('contains equal', function () {
    expect([['a' => 1], ['b' => 2]])->toContainEqual(['a' => 1]);
});

-----
<?php
class ToContainEqualTest extends \PHPUnit\Framework\TestCase
{
    public function test_contains_equal(): void
    {
        $this->assertContainsEquals(['a' => 1], [['a' => 1], ['b' => 2]]);
    }
}
