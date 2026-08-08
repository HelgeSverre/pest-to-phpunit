<?php
test('same subject different assertions', function () {
    expect($arr)->toBeArray();
    expect($arr)->toHaveCount(3);
    expect($arr)->toHaveKey('id');
    expect($arr)->toContain('value');
    expect($arr)->not->toBeEmpty();
});
-----
<?php
class MultipleExpectSameSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_same_subject_different_assertions(): void
    {
        $this->assertIsArray($arr);
        $this->assertCount(3, $arr);
        $this->assertArrayHasKey('id', $arr);
        if (is_string($__pest_contain_subject = $arr)) {
            $this->assertStringContainsString('value', $__pest_contain_subject);
        } else {
            $this->assertContains('value', $__pest_contain_subject);
        }
        $this->assertNotEmpty($arr);
    }
}
