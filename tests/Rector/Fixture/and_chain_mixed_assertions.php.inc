<?php
test('and mixed', function () {
    expect($arr)->toHaveCount(3)
        ->and($arr)->toHaveKey('id')
        ->and($arr)->toContain('value');
});
-----
<?php
class AndChainMixedAssertionsTest extends \PHPUnit\Framework\TestCase
{
    public function test_and_mixed(): void
    {
        $this->assertCount(3, $arr);
        $this->assertArrayHasKey('id', $arr);
        $this->assertContains('value', $arr);
    }
}
