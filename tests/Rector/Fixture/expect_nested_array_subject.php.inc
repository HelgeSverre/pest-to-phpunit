<?php
test('nested array', function () {
    expect(['users' => [['name' => 'John'], ['name' => 'Jane']]])
        ->toHaveKey('users');
});
-----
<?php
class ExpectNestedArraySubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_nested_array(): void
    {
        $this->assertArrayHasKey('users', ['users' => [['name' => 'John'], ['name' => 'Jane']]]);
    }
}
