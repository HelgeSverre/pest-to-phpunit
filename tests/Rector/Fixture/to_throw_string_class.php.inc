<?php
test('throws by class string', function () {
    expect(fn () => fail())->toThrow('RuntimeException');
});

test('throws by fqn string', function () {
    expect(fn () => fail())->toThrow('App\Exceptions\CustomException');
});

test('throws by message string', function () {
    expect(fn () => fail())->toThrow('something went wrong');
});
-----
<?php

class ToThrowStringClassTest extends \PHPUnit\Framework\TestCase
{
    public function test_throws_by_class_string(): void
    {
        $this->expectException(RuntimeException::class);
        (fn () => fail())();
    }
    public function test_throws_by_fqn_string(): void
    {
        $this->expectException(\App\Exceptions\CustomException::class);
        (fn () => fail())();
    }
    public function test_throws_by_message_string(): void
    {
        $this->expectExceptionMessage('something went wrong');
        (fn () => fail())();
    }
}
