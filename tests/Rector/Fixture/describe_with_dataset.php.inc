<?php
describe('Validation', function () {
    test('validates input', function (string $input, bool $expected) {
        expect(validate($input))->toBe($expected);
    })->with([
        ['valid@email.com', true],
        ['not-an-email', false],
    ]);
});
-----
<?php
class DescribeWithDatasetTest extends \PHPUnit\Framework\TestCase
{
    public static function test_validation_validates_input_provider(): array
    {
        return [
            ['valid@email.com', true],
            ['not-an-email', false],
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('test_validation_validates_input_provider')]
    public function test_validation_validates_input(string $input, bool $expected): void
    {
        $this->assertSame($expected, validate($input));
    }
}
