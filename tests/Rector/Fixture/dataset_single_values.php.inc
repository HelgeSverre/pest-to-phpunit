<?php
test('single value dataset', function (string $color) {
    expect($color)->toBeString();
})->with(['red', 'green', 'blue']);
-----
<?php
class DatasetSingleValuesTest extends \PHPUnit\Framework\TestCase
{
    public static function test_single_value_dataset_provider(): array
    {
        return [['red'], ['green'], ['blue']];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('test_single_value_dataset_provider')]
    public function test_single_value_dataset(string $color): void
    {
        $this->assertIsString($color);
    }
}
