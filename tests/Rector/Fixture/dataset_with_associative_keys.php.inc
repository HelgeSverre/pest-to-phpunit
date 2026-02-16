<?php
test('named dataset entries', function (string $name, int $age) {
    expect($name)->toBeString();
    expect($age)->toBeInt();
})->with([
    'young user' => ['Alice', 25],
    'old user' => ['Bob', 65],
]);
-----
<?php
class DatasetWithAssociativeKeysTest extends \PHPUnit\Framework\TestCase
{
    public static function named_dataset_entries_provider(): array
    {
        return [
            'young user' => ['Alice', 25],
            'old user' => ['Bob', 65],
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('named_dataset_entries_provider')]
    public function test_named_dataset_entries(string $name, int $age): void
    {
        $this->assertIsString($name);
        $this->assertIsInt($age);
    }
}
