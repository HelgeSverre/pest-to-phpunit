<?php
test('lowercase', function () {
    expect('hello')->toBeLowercase();
});
-----
<?php
class ToBeLowercaseTest extends \PHPUnit\Framework\TestCase
{
    public function test_lowercase(): void
    {
        $this->assertMatchesRegularExpression('/^[^A-Z]*$/', 'hello');
    }
}
