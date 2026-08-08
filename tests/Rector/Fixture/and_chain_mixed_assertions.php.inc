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
        if (is_string($__pest_contain_subject = $arr)) {
            $this->assertStringContainsString('value', $__pest_contain_subject);
        } else {
            $this->assertContains('value', $__pest_contain_subject);
        }
    }
}
