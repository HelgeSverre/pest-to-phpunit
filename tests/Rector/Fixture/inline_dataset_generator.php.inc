<?php

test('processes items', function (int $value) {
    expect($value)->toBeInt();
})->with(function () {
    yield [1];
    yield [2];
    yield [3];
});

-----
<?php

class InlineDatasetGeneratorTest extends \PHPUnit\Framework\TestCase
{
    public static function processes_items_provider(): iterable
    {
        yield [1];
        yield [2];
        yield [3];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('processes_items_provider')]
    public function test_processes_items(int $value): void
    {
        $this->assertIsInt($value);
    }
}
