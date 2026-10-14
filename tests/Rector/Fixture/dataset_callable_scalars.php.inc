<?php
dataset('arrow values', fn () => [1, [2]]);
dataset('closure values', function () {
    return [1, [2]];
});
dataset('generator values', function () {
    yield 'scalar' => 1;
    yield 'array' => [2];
});
test('arrow dataset', function (int $value) { expect($value)->toBeInt(); })->with('arrow values');
test('closure dataset', function (int $value) { expect($value)->toBeInt(); })->with('closure values');
test('generator dataset', function (int $value) { expect($value)->toBeInt(); })->with('generator values');
test('inline arrow dataset', function (int $value) { expect($value)->toBeInt(); })->with(fn () => [1, [2]]);
-----
<?php

class DatasetCallableScalarsTest extends \PHPUnit\Framework\TestCase
{
    public static function arrow_values(): array
    {
        return [[1], [2]];
    }
    public static function closure_values(): array
    {
        return [[1], [2]];
    }
    public static function generator_values(): iterable
    {
        yield 'scalar' => [1];
        yield 'array' => [2];
    }
    public static function inline_arrow_dataset_provider(): array
    {
        return [[1], [2]];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('arrow_values')]
    public function test_arrow_dataset(int $value): void
    {
        $this->assertIsInt($value);
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('closure_values')]
    public function test_closure_dataset(int $value): void
    {
        $this->assertIsInt($value);
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('generator_values')]
    public function test_generator_dataset(int $value): void
    {
        $this->assertIsInt($value);
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('inline_arrow_dataset_provider')]
    public function test_inline_arrow_dataset(int $value): void
    {
        $this->assertIsInt($value);
    }
}
