<?php
test('contains multiple', function () {
    expect([1, 2, 3, 4])->toContain(2, 4);
});
-----
<?php
class ToContainMultipleArgsTest extends \PHPUnit\Framework\TestCase
{
    public function test_contains_multiple(): void
    {
        if (is_string($__pest_contain_subject = [1, 2, 3, 4])) {
            $this->assertStringContainsString(2, $__pest_contain_subject);
        } else {
            $this->assertContains(2, $__pest_contain_subject);
        }
        if (is_string($__pest_contain_subject = [1, 2, 3, 4])) {
            $this->assertStringContainsString(4, $__pest_contain_subject);
        } else {
            $this->assertContains(4, $__pest_contain_subject);
        }
    }
}
