<?php
test('parameterized grouped', function (int $n) {
    expect($n)->toBeGreaterThan(0);
})->with([1, 2, 3])->group('math');
-----
<?php
class WithAndGroupCombinedTest extends \PHPUnit\Framework\TestCase
{
    public static function test_parameterized_grouped_provider(): array
    {
        return [1, 2, 3];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('test_parameterized_grouped_provider')]
    #[\PHPUnit\Framework\Attributes\Group('math')]
    public function test_parameterized_grouped(int $n): void
    {
        $this->assertGreaterThan(0, $n);
    }
}
