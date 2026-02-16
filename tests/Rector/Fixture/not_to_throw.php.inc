<?php

test('does not throw', function () {
    expect(fn () => doSomething())->not->toThrow(RuntimeException::class);
});

-----
<?php

class NotToThrowTest extends \PHPUnit\Framework\TestCase
{
    public function test_does_not_throw(): void
    {
        (fn () => doSomething())();
    }
}
