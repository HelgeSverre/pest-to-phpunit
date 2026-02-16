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
        try {
            (fn () => doSomething())();
            $this->addToAssertionCount(1);
        } catch (RuntimeException $__exception) {
            $this->fail('Expected no exception, but ' . get_class($__exception) . ' was thrown: ' . $__exception->getMessage());
        }
    }
}
