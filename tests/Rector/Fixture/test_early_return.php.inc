<?php
test('early return', function () {
    $value = getValue();
    if ($value === null) {
        expect($value)->toBeNull();
        return;
    }
    expect($value)->toBeString();
    expect(strlen($value))->toBeGreaterThan(0);
});
-----
<?php
class TestEarlyReturnTest extends \PHPUnit\Framework\TestCase
{
    public function test_early_return(): void
    {
        $value = getValue();
        if ($value === null) {
            $this->assertNull($value);
            return;
        }
        $this->assertIsString($value);
        $this->assertGreaterThan(0, strlen($value));
    }
}
