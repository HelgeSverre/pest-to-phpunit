<?php
test('nested closures', function () {
    $filter = function (array $items) {
        return array_filter($items, function ($item) {
            return $item > 0;
        });
    };

    expect($filter([-1, 0, 1, 2]))->toBe([2 => 1, 3 => 2]);
});
-----
<?php
class TestWithNestedClosuresTest extends \PHPUnit\Framework\TestCase
{
    public function test_nested_closures(): void
    {
        $filter = function (array $items) {
            return array_filter($items, function ($item) {
                return $item > 0;
            });
        };
        $this->assertSame([2 => 1, 3 => 2], $filter([-1, 0, 1, 2]));
    }
}
