<?php
test('with closure var', function () {
    $transform = fn ($x) => $x * 2;
    expect($transform(5))->toBe(10);
});
-----
<?php
class TestWithClosureVariableTest extends \PHPUnit\Framework\TestCase
{
    public function test_with_closure_var(): void
    {
        $transform = fn ($x) => $x * 2;
        $this->assertSame(10, $transform(5));
    }
}
