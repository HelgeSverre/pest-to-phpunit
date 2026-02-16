<?php
describe('Math', function () {
    test('doubles', function (int $input, int $expected) {
        expect($input * 2)->toBe($expected);
    })->with([
        [1, 2],
        [5, 10],
    ]);
});
-----
<?php
class DescribeWithInlineDatasetTest extends \PHPUnit\Framework\TestCase
{
    public static function math_doubles_provider(): array
    {
        return [
            [1, 2],
            [5, 10],
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('math_doubles_provider')]
    public function test_math_doubles(int $input, int $expected): void
    {
        $this->assertSame($expected, $input * 2);
    }
}
