<?php
test('complex dataset values', function (array $config) {
    expect($config)->toHaveKey('host');
})->with([
    [['host' => 'localhost', 'port' => 3306]],
    [['host' => '127.0.0.1', 'port' => 5432]],
]);
-----
<?php
class DatasetComplexValuesTest extends \PHPUnit\Framework\TestCase
{
    public static function test_complex_dataset_values_provider(): array
    {
        return [
            [['host' => 'localhost', 'port' => 3306]],
            [['host' => '127.0.0.1', 'port' => 5432]],
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('test_complex_dataset_values_provider')]
    public function test_complex_dataset_values(array $config): void
    {
        $this->assertArrayHasKey('host', $config);
    }
}
