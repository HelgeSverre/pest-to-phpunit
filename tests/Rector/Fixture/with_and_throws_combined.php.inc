<?php
test('throws per dataset', function (string $input) {
    processInput($input);
})->with(['', null, false])->throws(InvalidArgumentException::class);
-----
<?php
class WithAndThrowsCombinedTest extends \PHPUnit\Framework\TestCase
{
    public static function test_throws_per_dataset_provider(): array
    {
        return ['', null, false];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('test_throws_per_dataset_provider')]
    public function test_throws_per_dataset(string $input): void
    {
        $this->expectException(InvalidArgumentException::class);
        processInput($input);
    }
}
