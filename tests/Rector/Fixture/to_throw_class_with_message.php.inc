<?php
test('throws with class and message', function () {
    expect(fn () => throwError())->toThrow(RuntimeException::class, 'Something went wrong');
});

test('throws with string class and message', function () {
    expect(fn () => throwError())->toThrow('RuntimeException', 'Bad input');
});

-----
<?php

class ToThrowClassWithMessageTest extends \PHPUnit\Framework\TestCase
{
    public function test_throws_with_class_and_message(): void
    {
        $this->expectException(RuntimeException::class);
        $this->expectExceptionMessage('Something went wrong');
        (fn () => throwError())();
    }
    public function test_throws_with_string_class_and_message(): void
    {
        $this->expectException(RuntimeException::class);
        $this->expectExceptionMessage('Bad input');
        (fn () => throwError())();
    }
}
