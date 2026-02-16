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
            $this->assertContains('error', $e->getMessage());
        }
    }
}
