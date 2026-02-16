<?php
dataset('numbers', [1, 2, 3]);

dataset('letters', ['a', 'b', 'c']);

test('uses numbers', function (int $n) {
    expect($n)->toBeInt();
})->with('numbers');

test('uses letters', function (string $l) {
    expect($l)->toBeString();
})->with('letters');
-----
<?php

class MultipleDatasetsNamedTest extends \PHPUnit\Framework\TestCase
{
    public static function numbers(): array
    {
        return [1, 2, 3];
    }
    public static function letters(): array
    {
        return ['a', 'b', 'c'];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('numbers')]
    public function test_uses_numbers(int $n): void
    {
        $this->assertIsInt($n);
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('letters')]
    public function test_uses_letters(string $l): void
    {
        $this->assertIsString($l);
    }
}
