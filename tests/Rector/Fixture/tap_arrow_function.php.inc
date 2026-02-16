<?php
test('tap with arrow function', function () {
    expect($result)->tap(fn ($value) => expect($value)->toBeGreaterThan(0))->toBe(42);
});
-----
<?php
class TapArrowFunctionTest extends \PHPUnit\Framework\TestCase
{
    public function test_tap_with_arrow_function(): void
    {
        $value = $result;
        $this->assertGreaterThan(0, $value);
        $this->assertSame(42, $result);
    }
}
