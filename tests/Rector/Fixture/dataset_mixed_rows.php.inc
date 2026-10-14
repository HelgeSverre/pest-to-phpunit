<?php
dataset('values', ['scalar' => 1, 'array' => [2]]);
test('named rows', function (int $value) {
    expect($value)->toBeInt();
})->with('values');
test('inline rows', function (int $value) {
    expect($value)->toBeInt();
})->with(['scalar' => 1, 'array' => [2]]);
-----
<?php

class DatasetMixedRowsTest extends \PHPUnit\Framework\TestCase
{
    public static function values(): array
    {
        return ['scalar' => [1], 'array' => [2]];
    }
    public static function inline_rows_provider(): array
    {
        return ['scalar' => [1], 'array' => [2]];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('values')]
    public function test_named_rows(int $value): void
    {
        $this->assertIsInt($value);
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('inline_rows_provider')]
    public function test_inline_rows(int $value): void
    {
        $this->assertIsInt($value);
    }
}
