<?php
test('empty dataset', function ($x) {
    expect($x)->toBeNull();
})->with([]);
-----
<?php
class DatasetEmptyArrayTest extends \PHPUnit\Framework\TestCase
{
    public static function test_empty_dataset_provider(): array
    {
        return [];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('test_empty_dataset_provider')]
    public function test_empty_dataset($x): void
    {
        $this->assertNull($x);
    }
}
