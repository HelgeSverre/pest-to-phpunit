<?php
test('message ending with Error', function () {
    expect(fn () => oops())->toThrow('An Error');
});

test('message ending with Exception', function () {
    expect(fn () => oops())->toThrow('Caught a RuntimeException');
});
-----
<?php

class ToThrowMessageEndingErrorTest extends \PHPUnit\Framework\TestCase
{
    public function test_message_ending_with_error(): void
    {
        $this->expectExceptionMessage('An Error');
        (fn () => oops())();
    }
    public function test_message_ending_with_exception(): void
    {
        $this->expectExceptionMessage('Caught a RuntimeException');
        (fn () => oops())();
    }
}
