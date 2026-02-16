<?php
test('value in list', function () {
    expect('a')->toBeIn(['a', 'b', 'c']);
});

-----
<?php
class ToBeInTest extends \PHPUnit\Framework\TestCase
{
    public function test_value_in_list(): void
    {
        $this->assertContains('a', ['a', 'b', 'c']);
    }
}
