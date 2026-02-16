<?php

test('throws exception', function () {
    expect(fn () => throw new RuntimeException('boom'))
        ->toThrow(RuntimeException::class, 'boom');
});

-----
<?php

class ReadmeToThrowTest extends \PHPUnit\Framework\TestCase
{
    public function test_throws_exception(): void
    {
        $this->expectException(RuntimeException::class);
        $this->expectExceptionMessage('boom');
        (fn () => throw new RuntimeException('boom'))();
    }
}
