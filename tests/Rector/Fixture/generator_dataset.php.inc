<?php

dataset('names', function () {
    yield 'alice' => ['Alice'];
    yield 'bob' => ['Bob'];
});

test('greets user', function (string $name) {
    expect($name)->toBeString();
})->with('names');

-----
<?php

class GeneratorDatasetTest extends \PHPUnit\Framework\TestCase
{
    public static function names(): iterable
    {
        yield 'alice' => ['Alice'];
        yield 'bob' => ['Bob'];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('names')]
    public function test_greets_user(string $name): void
    {
        $this->assertIsString($name);
    }
}
