<?php
test('throw with message', function () {
    expect(fn () => fail('oops'))->toThrow(RuntimeException::class, 'oops');
});

-----
<?php
class ExpectClosureSubjectToThrowMessageTest extends \PHPUnit\Framework\TestCase
{
    public function test_throw_with_message(): void
    {
        $this->expectException(RuntimeException::class);
        $this->expectExceptionMessage('oops');
        (fn () => fail('oops'))();
    }
}
