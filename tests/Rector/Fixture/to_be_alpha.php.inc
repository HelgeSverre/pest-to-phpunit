<?php
test('alpha', function () {
    expect('Hello')->toBeAlpha();
});
-----
<?php
class ToBeAlphaTest extends \PHPUnit\Framework\TestCase
{
    public function test_alpha(): void
    {
        $this->assertMatchesRegularExpression('/^[a-zA-Z]+$/', 'Hello');
    }
}
