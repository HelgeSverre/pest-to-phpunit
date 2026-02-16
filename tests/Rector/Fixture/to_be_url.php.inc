<?php
test('url check', function () {
    expect('https://example.com')->toBeUrl();
});
-----
<?php
class ToBeUrlTest extends \PHPUnit\Framework\TestCase
{
    public function test_url_check(): void
    {
        $this->assertMatchesRegularExpression('/^https?:\/\/.+/', 'https://example.com');
    }
}
