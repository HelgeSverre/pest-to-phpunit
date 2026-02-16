<?php
test('does cleanup', function () {
    expect(true)->toBeTrue();
})->after(fn () => cleanupTempFiles());

-----
<?php
class AfterArrowFunctionTest extends \PHPUnit\Framework\TestCase
{
    public function test_does_cleanup(): void
    {
        try {
            $this->assertTrue(true);
        } finally {
            cleanupTempFiles();
        }
    }
}

