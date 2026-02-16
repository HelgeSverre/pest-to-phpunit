<?php
test('digits only', function () {
    expect('12345')->toBeDigits();
});
-----
<?php
class ToBeDigitsTest extends \PHPUnit\Framework\TestCase
{
    public function test_digits_only(): void
    {
        $this->assertMatchesRegularExpression('/^\d+$/', '12345');
    }
}
