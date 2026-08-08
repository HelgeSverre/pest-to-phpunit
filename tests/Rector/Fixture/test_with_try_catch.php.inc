<?php
test('handles exceptions', function () {
    try {
        riskyOperation();
        expect(true)->toBeTrue();
    } catch (Exception $e) {
        expect($e->getMessage())->toContain('error');
    }
});
-----
<?php
class TestWithTryCatchTest extends \PHPUnit\Framework\TestCase
{
    public function test_handles_exceptions(): void
    {
        try {
            riskyOperation();
            $this->assertTrue(true);
        } catch (Exception $e) {
            if (is_string($__pest_contain_subject = $e->getMessage())) {
                $this->assertStringContainsString('error', $__pest_contain_subject);
            } else {
                $this->assertContains('error', $__pest_contain_subject);
            }
        }
    }
}
