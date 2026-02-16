<?php
test('throws with message only', function () {
    expect(fn () => fail('oops'))->toThrow('oops');
});

-----
<?php
class ToThrowStringMessageTest extends \PHPUnit\Framework\TestCase
{
    public function test_throws_with_message_only(): void
    {
        $this->expectExceptionMessage('oops');
        (fn () => fail('oops'))();
    }
}
