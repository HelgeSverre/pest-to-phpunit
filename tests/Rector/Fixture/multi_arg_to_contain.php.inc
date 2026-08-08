<?php
test('contains multiple', function () {
    expect($items)->toContain('a', 'b', 'c');
});
-----
<?php
class MultiArgToContainTest extends \PHPUnit\Framework\TestCase
{
    public function test_contains_multiple(): void
    {
        if (is_string($__pest_contain_subject = $items)) {
            $this->assertStringContainsString('a', $__pest_contain_subject);
        } else {
            $this->assertContains('a', $__pest_contain_subject);
        }
        if (is_string($__pest_contain_subject = $items)) {
            $this->assertStringContainsString('b', $__pest_contain_subject);
        } else {
            $this->assertContains('b', $__pest_contain_subject);
        }
        if (is_string($__pest_contain_subject = $items)) {
            $this->assertStringContainsString('c', $__pest_contain_subject);
        } else {
            $this->assertContains('c', $__pest_contain_subject);
        }
    }
}
