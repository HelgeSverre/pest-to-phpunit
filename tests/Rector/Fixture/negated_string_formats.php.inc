<?php
test('negated string formats', function () {
    expect('hello')->not->toBeUppercase();
    expect('HELLO')->not->toBeLowercase();
});
-----
<?php
class NegatedStringFormatsTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_string_formats(): void
    {
        $this->assertNotSame(strtoupper('hello'), 'hello');
        $this->assertNotSame(strtolower('HELLO'), 'HELLO');
    }
}
