<?php
test('math expression subject', function () {
    expect(2 + 3 * 4)->toBe(14);
    expect(10 / 2)->toBe(5);
    expect(10 % 3)->toBe(1);
});
-----
<?php
class ExpectWithMathSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_math_expression_subject(): void
    {
        $this->assertSame(14, 2 + 3 * 4);
        $this->assertSame(5, 10 / 2);
        $this->assertSame(1, 10 % 3);
    }
}
