<?php
test('throws instance', function () {
    expect(fn () => fail())->toThrow(new RuntimeException('oops'));
});

-----
<?php
class ToThrowInstanceTest extends \PHPUnit\Framework\TestCase
{
    public function test_throws_instance(): void
    {
        $this->expectException(RuntimeException::class);
        $this->expectExceptionMessage('oops');
        (fn () => fail())();
    }
}
