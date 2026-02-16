<?php
test('negated list and contain equal', function () {
    expect($x)->not->toContainEqual('item');
});
-----
<?php
class NegatedListContainEqualTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_list_and_contain_equal(): void
    {
        $this->assertNotContainsEquals('item', $x);
    }
}
