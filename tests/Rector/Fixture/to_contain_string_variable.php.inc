<?php
test('contains substring in variable', function () {
    $value = 'alphabet';
    expect($value)->toContain('pha');
});
-----
<?php
class ToContainStringVariableTest extends \PHPUnit\Framework\TestCase
{
    public function test_contains_substring_in_variable(): void
    {
        $value = 'alphabet';
        if (is_string($__pest_contain_subject = $value)) {
            $this->assertStringContainsString('pha', $__pest_contain_subject);
        } else {
            $this->assertContains('pha', $__pest_contain_subject);
        }
    }
}
