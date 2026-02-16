<?php
test('array destructuring', function () {
    [$first, $second] = getValues();
    expect($first)->toBe(1);
    expect($second)->toBe(2);
});
-----
<?php
class TestWithArrayDestructuringTest extends \PHPUnit\Framework\TestCase
{
    public function test_array_destructuring(): void
    {
        [$first, $second] = getValues();
        $this->assertSame(1, $first);
        $this->assertSame(2, $second);
    }
}
