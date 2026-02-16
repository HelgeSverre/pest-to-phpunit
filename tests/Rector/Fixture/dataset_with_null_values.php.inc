<?php
test('null in dataset', function ($value) {
    expect($value)->toBeNull();
})->with([
    [null],
]);
-----
<?php
class DatasetWithNullValuesTest extends \PHPUnit\Framework\TestCase
{
    public static function null_in_dataset_provider(): array
    {
        return [
            [null],
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('null_in_dataset_provider')]
    public function test_null_in_dataset($value): void
    {
        $this->assertNull($value);
    }
}
