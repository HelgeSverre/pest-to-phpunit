<?php

test('does not throw anything', function () {
    expect(fn () => safeOperation())->not->toThrow();
});

-----
<?php

class NotToThrowNoClassTest extends \PHPUnit\Framework\TestCase
{
    public function test_does_not_throw_anything(): void
    {
        try {
            (fn () => safeOperation())();
            $this->addToAssertionCount(1);
        } catch (\Throwable $__exception) {
            $this->fail('Expected no exception, but ' . get_class($__exception) . ' was thrown: ' . $__exception->getMessage());
        }
    }
}
