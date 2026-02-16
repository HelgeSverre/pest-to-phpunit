<?php
test('doubles number', function (int $input, int $expected) {
    expect($input * 2)->toBe($expected);
})->with([
    [1, 2],
    [2, 4],
    [3, 6],
]);

-----
<?php
class InlineWithArrayTest extends \PHPUnit\Framework\TestCase
{
    public static function doubles_number_provider(): array
    {
        return [
            [1, 2],
            [2, 4],
            [3, 6],
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('doubles_number_provider')]
    public function test_doubles_number(int $input, int $expected): void
    {
        $this->assertSame($expected, $input * 2);
    }
}
