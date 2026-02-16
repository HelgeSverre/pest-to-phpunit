<?php
test('each equal', function () {
    expect($items)->each->toEqual('expected');
});
-----
<?php
class EachToEqualTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_equal(): void
    {
        foreach ($items as $__each_item) {
            $this->assertEquals('expected', $__each_item);
        }
    }
}
