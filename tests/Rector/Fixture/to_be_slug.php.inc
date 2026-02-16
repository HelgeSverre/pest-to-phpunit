<?php
test('slug format', function () {
    expect('hello-world-123')->toBeKebabCase();
});
-----
<?php
class ToBeSlugTest extends \PHPUnit\Framework\TestCase
{
    public function test_slug_format(): void
    {
        $this->assertMatchesRegularExpression('/^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$/', 'hello-world-123');
    }
}
