<?php
test('contains in property value', function () {
    expect($user)->name->toContain('John');
});
-----
<?php
class ToContainAfterPropertyAccessTest extends \PHPUnit\Framework\TestCase
{
    public function test_contains_in_property_value(): void
    {
        if (is_string($__pest_contain_subject = $user->name)) {
            $this->assertStringContainsString('John', $__pest_contain_subject);
        } else {
            $this->assertContains('John', $__pest_contain_subject);
        }
    }
}
