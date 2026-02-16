<?php
test('value not in list', function () {
    expect('d')->not->toBeIn(['a', 'b', 'c']);
});

-----
<?php
class NotToBeInTest extends \PHPUnit\Framework\TestCase
{
    public function test_value_not_in_list(): void
    {
        $this->assertNotContains('d', ['a', 'b', 'c']);
    }
}
