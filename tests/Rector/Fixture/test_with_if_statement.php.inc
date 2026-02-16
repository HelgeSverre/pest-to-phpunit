<?php
test('with control flow', function () {
    $value = computeValue();

    if ($value > 0) {
        expect($value)->toBeGreaterThan(0);
    } else {
        expect($value)->toBeLessThanOrEqual(0);
    }
});
-----
<?php
class TestWithIfStatementTest extends \PHPUnit\Framework\TestCase
{
    public function test_with_control_flow(): void
    {
        $value = computeValue();
        if ($value > 0) {
        } else {
        }
    }
}
