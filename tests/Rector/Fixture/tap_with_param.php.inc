<?php
test('tap with param', function () {
    expect($result)->tap(function ($value) {
        expect($value)->toBeGreaterThan(0);
    })->toBe(42);
});
-----
<?php
class TapWithParamTest extends \PHPUnit\Framework\TestCase
{
    public function test_tap_with_param(): void
    {
        $value = $result;
        $this->assertGreaterThan(0, $value);
        $this->assertSame(42, $result);
    }
}
