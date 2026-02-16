<?php
describe('Parser', function () {
    test('parses JSON', function () {
        expect(json_decode('{}', true))->toBeArray();
    });

    it('handles invalid JSON', function () {
        expect(json_decode('invalid', true))->toBeNull();
    });

    test('parses arrays', function () {
        expect(json_decode('[1,2]', true))->toBe([1, 2]);
    });

    it('returns null for empty input', function () {
        expect(json_decode('', true))->toBeNull();
    });
});
-----
<?php
class DescribeMixedItAndTest extends \PHPUnit\Framework\TestCase
{
    public function test_parser_parses_json(): void
    {
        $this->assertIsArray(json_decode('{}', true));
    }
    public function test_it_parser_handles_invalid_json(): void
    {
        $this->assertNull(json_decode('invalid', true));
    }
    public function test_parser_parses_arrays(): void
    {
        $this->assertSame([1, 2], json_decode('[1,2]', true));
    }
    public function test_it_parser_returns_null_for_empty_input(): void
    {
        $this->assertNull(json_decode('', true));
    }
}
