<?php
dataset('items', function () {
    return [
        ['apple'],
        ['banana'],
    ];
});

test('has items', function (string $item) {
    expect($item)->toBeString();
})->with('items');

-----
<?php

class DatasetClosureNoYieldTest extends \PHPUnit\Framework\TestCase
{
    public static function items(): array
    {
        return [
            ['apple'],
            ['banana'],
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('items')]
    public function test_has_items(string $item): void
    {
        $this->assertIsString($item);
    }
}
