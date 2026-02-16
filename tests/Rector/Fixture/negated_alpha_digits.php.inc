<?php
test('negated alpha digits', function () {
    expect('123')->not->toBeAlpha();
    expect('abc')->not->toBeDigits();
    expect('abc!@#')->not->toBeAlphaNumeric();
});
-----
<?php
class NegatedAlphaDigitsTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_alpha_digits(): void
    {
        $this->assertDoesNotMatchRegularExpression('/^[a-zA-Z]+$/', '123');
        $this->assertDoesNotMatchRegularExpression('/^\d+$/', 'abc');
        $this->assertDoesNotMatchRegularExpression('/^[a-zA-Z0-9]+$/', 'abc!@#');
    }
}
