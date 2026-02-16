<?php
test('with custom message', function () {
    expect($value)->toBe(42, 'Value should be 42');
    expect($name)->toEqual('John', 'Name mismatch');
});

-----
<?php
class ExpectWithCustomMessageTest extends \PHPUnit\Framework\TestCase
{
    public function test_with_custom_message(): void
    {
        $this->assertSame(42, $value, 'Value should be 42');
        $this->assertEquals('John', $name, 'Name mismatch');
    }
}
