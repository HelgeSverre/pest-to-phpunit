<?php
test('handles übersetzung and données', function () {
    expect(true)->toBeTrue();
});

-----
<?php
class TestUnicodeDescriptionTest extends \PHPUnit\Framework\TestCase
{
    public function test_handles_bersetzung_and_donnes(): void
    {
        $this->assertTrue(true);
    }
}
