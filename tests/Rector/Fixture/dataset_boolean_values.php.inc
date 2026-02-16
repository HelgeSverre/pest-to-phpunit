<?php
test('boolean dataset', function (bool $flag) {
    expect($flag)->toBeBool();
})->with([
    [true],
    [false],
]);
-----
<?php
class DatasetBooleanValuesTest extends \PHPUnit\Framework\TestCase
{
    public static function boolean_dataset_provider(): array
    {
        return [
            [true],
            [false],
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('boolean_dataset_provider')]
    public function test_boolean_dataset(bool $flag): void
    {
        $this->assertIsBool($flag);
    }
}
