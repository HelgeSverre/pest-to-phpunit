<?php
test('closure provider', function (int $n) {
    expect($n)->toBeInt();
})->with(function () {
    return [
        [1],
        [2],
        [3],
    ];
});
-----
<?php
class InlineWithClosureReturnTest extends \PHPUnit\Framework\TestCase
{
    public static function closure_provider_provider(): array
    {
        return [
            [1],
            [2],
            [3],
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('closure_provider_provider')]
    public function test_closure_provider(int $n): void
    {
        $this->assertIsInt($n);
    }
}
