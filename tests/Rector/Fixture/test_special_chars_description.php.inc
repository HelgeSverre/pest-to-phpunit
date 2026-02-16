<?php
test('it handles quotes and angles correctly', function () {
    expect(true)->toBeTrue();
});
-----
<?php
class TestSpecialCharsDescriptionTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_handles_quotes_and_angles_correctly(): void
    {
        $this->assertTrue(true);
    }
}
