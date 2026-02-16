<?php
test('alphanumeric', function () {
    expect('abc123')->toBeAlphaNumeric();
});
-----
<?php
class ToBeAlphaNumericTest extends \PHPUnit\Framework\TestCase
{
    public function test_alphanumeric(): void
    {
        $this->assertMatchesRegularExpression('/^[a-zA-Z0-9]+$/', 'abc123');
    }
}
