<?php
test('match expression', function () {
    $result = match ($status) {
        'active' => true,
        'inactive' => false,
        default => null,
    };
    expect($result)->toBeTrue();
});
-----
<?php
class TestWithMatchExpressionTest extends \PHPUnit\Framework\TestCase
{
    public function test_match_expression(): void
    {
        $result = match ($status) {
            'active' => true,
            'inactive' => false,
            default => null,
        };
        $this->assertTrue($result);
    }
}
