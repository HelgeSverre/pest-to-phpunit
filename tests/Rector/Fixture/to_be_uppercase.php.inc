<?php
test('string format checks', function () {
    expect('HELLO')->toBeUppercase();
    expect('hello')->toBeLowercase();
    expect('Hello')->toBeAlpha();
    expect('abc123')->toBeAlphaNumeric();
});

-----
<?php
class ToBeUppercaseTest extends \PHPUnit\Framework\TestCase
{
    public function test_string_format_checks(): void
    {
        $this->assertMatchesRegularExpression('/^[^a-z]*$/', 'HELLO');
        $this->assertMatchesRegularExpression('/^[^A-Z]*$/', 'hello');
        $this->assertMatchesRegularExpression('/^[a-zA-Z]+$/', 'Hello');
        $this->assertMatchesRegularExpression('/^[a-zA-Z0-9]+$/', 'abc123');
    }
}
