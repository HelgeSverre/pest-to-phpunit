<?php
test('is list', function () {
    expect([1, 2, 3])->toBeList();
});

-----
<?php
class ToBeListTest extends \PHPUnit\Framework\TestCase
{
    public function test_is_list(): void
    {
        $this->assertIsList([1, 2, 3]);
    }
}
