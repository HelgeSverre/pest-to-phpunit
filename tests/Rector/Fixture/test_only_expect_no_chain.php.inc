<?php
test('standalone expect call', function () {
    $result = compute();
    expect($result)->toBeArray();
    expect($result)->toHaveCount(3);
    expect($result)->toHaveKey('id');
});
-----
<?php
class TestOnlyExpectNoChainTest extends \PHPUnit\Framework\TestCase
{
    public function test_standalone_expect_call(): void
    {
        $result = compute();
        $this->assertIsArray($result);
        $this->assertCount(3, $result);
        $this->assertArrayHasKey('id', $result);
    }
}
