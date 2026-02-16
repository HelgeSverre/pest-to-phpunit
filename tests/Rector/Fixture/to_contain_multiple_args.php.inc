<?php
test('contains multiple', function () {
    expect([1, 2, 3, 4])->toContain(2, 4);
});

-----
<?php
class ToContainMultipleArgsTest extends \PHPUnit\Framework\TestCase
{
    public function test_contains_multiple(): void
    {
        $this->assertContains(2, [1, 2, 3, 4]);
        $this->assertContains(4, [1, 2, 3, 4]);
    }
}
