<?php
describe('Math', function () {
    test('addition', function () {
        expect(1 + 1)->toBe(2);
    });

    test('subtraction', function () {
        expect(5 - 3)->toBe(2);
    });
});
-----
<?php
class DescribeWithTestFuncTest extends \PHPUnit\Framework\TestCase
{
    public function test_math_addition(): void
    {
        $this->assertSame(2, 1 + 1);
    }
    public function test_math_subtraction(): void
    {
        $this->assertSame(2, 5 - 3);
    }
}
