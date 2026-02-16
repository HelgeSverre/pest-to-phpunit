<?php
test('throws something', function () {
    expect(fn () => riskyOperation())->toThrow();
});
-----
<?php
class ToThrowNoArgsTest extends \PHPUnit\Framework\TestCase
{
    public function test_throws_something(): void
    {
        $this->expectException(\Throwable::class);
        (fn () => riskyOperation())();
    }
}
