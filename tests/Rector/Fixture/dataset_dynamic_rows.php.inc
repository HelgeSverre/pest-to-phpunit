<?php
const ARRAY_ROW = [2];
dataset('dynamic rows', function () {
    $__pest_dataset_row = 'original';
    yield ARRAY_ROW;
    yield strlen('abc');
    yield strlen($__pest_dataset_row);
});
test('dynamic rows', function (int $value) {
    expect($value)->toBeIn([2, 3, 8]);
})->with('dynamic rows');
-----
<?php
const ARRAY_ROW = [2];
class DatasetDynamicRowsTest extends \PHPUnit\Framework\TestCase
{
    public static function dynamic_rows(): iterable
    {
        $__pest_dataset_row = 'original';
        yield (static fn($__pest_dataset_row) => is_array($__pest_dataset_row) ? $__pest_dataset_row : [$__pest_dataset_row])(ARRAY_ROW);
        yield (static fn($__pest_dataset_row) => is_array($__pest_dataset_row) ? $__pest_dataset_row : [$__pest_dataset_row])(strlen('abc'));
        yield (static fn($__pest_dataset_row) => is_array($__pest_dataset_row) ? $__pest_dataset_row : [$__pest_dataset_row])(strlen($__pest_dataset_row));
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('dynamic_rows')]
    public function test_dynamic_rows(int $value): void
    {
        $this->assertContains($value, [2, 3, 8]);
    }
}
