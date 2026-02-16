<?php
test('arrow throw', function () {
    expect(fn () => throw new \LogicException('nope'))
        ->toThrow(\LogicException::class, 'nope');
});
-----
<?php
class ToThrowArrowFunctionTest extends \PHPUnit\Framework\TestCase
{
    public function test_arrow_throw(): void
    {
        $this->expectException(\LogicException::class);
        $this->expectExceptionMessage('nope');
        (fn () => throw new \LogicException('nope'))();
    }
}
