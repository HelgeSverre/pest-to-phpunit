<?php
test('expect with expression', function () {
    expect(array_sum([1, 2, 3]))->toBe(6);
});
-----
<?php
class ExpectExpressionSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_expect_with_expression(): void
    {
        $this->assertSame(6, array_sum([1, 2, 3]));
    }
}
