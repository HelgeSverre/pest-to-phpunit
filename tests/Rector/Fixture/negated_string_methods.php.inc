<?php
test('negated string methods', function () {
    expect($s)->not->toStartWith('foo');
    expect($s)->not->toEndWith('bar');
    expect($s)->not->toMatch('/pattern/');
});
-----
<?php
class NegatedStringMethodsTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_string_methods(): void
    {
        $this->assertStringStartsNotWith('foo', $s);
        $this->assertStringEndsNotWith('bar', $s);
        $this->assertDoesNotMatchRegularExpression('/pattern/', $s);
    }
}
