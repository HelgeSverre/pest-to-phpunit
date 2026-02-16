<?php
test('with switch', function () {
    $result = match(true) {
        is_string($value) => 'string',
        is_int($value) => 'int',
        default => 'other',
    };
    expect($result)->toBeString();
});
-----
<?php
class TestWithSwitchTest extends \PHPUnit\Framework\TestCase
{
    public function test_with_switch(): void
    {
        $result = match(true) {
            is_string($value) => 'string',
            is_int($value) => 'int',
            default => 'other',
        };
        $this->assertIsString($result);
    }
}
