<?php
test('complex closure throw', function () {
    expect(function () {
        $service = new Service();
        $service->process(null);
    })->toThrow(InvalidArgumentException::class);
});
-----
<?php
class ToThrowComplexClosureTest extends \PHPUnit\Framework\TestCase
{
    public function test_complex_closure_throw(): void
    {
        $this->expectException(InvalidArgumentException::class);
        (function () {
            $service = new Service();
            $service->process(null);
        })();
    }
}
