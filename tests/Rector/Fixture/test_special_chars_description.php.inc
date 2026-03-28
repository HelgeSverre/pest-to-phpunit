<?php
test('handles "quotes" & <special> chars', function () {
    expect(true)->toBeTrue();
});

-----
<?php
class TestSpecialCharsDescriptionTest extends \PHPUnit\Framework\TestCase
{
    public function test_handles_quotes_special_chars(): void
    {
        $this->assertTrue(true);
    }
}
