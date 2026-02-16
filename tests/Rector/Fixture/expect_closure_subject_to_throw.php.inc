<?php
test('throw only class', function () {
    expect(fn () => methodThatThrows())->toThrow(RuntimeException::class);
});

-----
<?php
class ExpectClosureSubjectToThrowTest extends \PHPUnit\Framework\TestCase
{
    public function test_throw_only_class(): void
    {
        $this->expectException(RuntimeException::class);
        (fn () => methodThatThrows())();
    }
}
