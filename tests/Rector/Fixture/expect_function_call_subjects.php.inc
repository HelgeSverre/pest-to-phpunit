<?php
test('function call subject', function () {
    expect(strlen('hello'))->toBe(5);
    expect(count([1, 2, 3]))->toBe(3);
    expect(strtolower('HELLO'))->toBe('hello');
});
-----
<?php
class ExpectFunctionCallSubjectsTest extends \PHPUnit\Framework\TestCase
{
    public function test_function_call_subject(): void
    {
        $this->assertSame(5, strlen('hello'));
        $this->assertSame(3, count([1, 2, 3]));
        $this->assertSame('hello', strtolower('HELLO'));
    }
}
