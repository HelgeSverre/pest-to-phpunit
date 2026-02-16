<?php

test('cross join', function ($a, $b) {
    expect($a + $b)->toBeGreaterThan(0);
})->with([1, 2])->with([10, 20]);

-----
<?php

class MultipleWithCrossJoinTest extends \PHPUnit\Framework\TestCase
{
    public static function test_cross_join_provider(): array
    {
        return [[1], [2]];
    }
    public static function test_cross_join_provider_2(): array
    {
        return [[10], [20]];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('test_cross_join_provider')]
    #[\PHPUnit\Framework\Attributes\DataProvider('test_cross_join_provider_2')]
    public function test_cross_join($a, $b): void
    {
        $this->assertGreaterThan(0, $a + $b);
    }
}
