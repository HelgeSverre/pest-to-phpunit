<?php
test('null coalesce', function () {
    $value = $config['key'] ?? 'default';
    expect($value)->toBe('default');
});
-----
<?php
class TestWithNullCoalesceTest extends \PHPUnit\Framework\TestCase
{
    public function test_null_coalesce(): void
    {
        $value = $config['key'] ?? 'default';
        $this->assertSame('default', $value);
    }
}
