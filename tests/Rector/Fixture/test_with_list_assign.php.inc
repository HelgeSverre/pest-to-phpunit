<?php
test('list assignment', function () {
    ['name' => $name, 'age' => $age] = getUserData();
    expect($name)->toBeString();
    expect($age)->toBeInt();
});
-----
<?php
class TestWithListAssignTest extends \PHPUnit\Framework\TestCase
{
    public function test_list_assignment(): void
    {
        ['name' => $name, 'age' => $age] = getUserData();
        $this->assertIsString($name);
        $this->assertIsInt($age);
    }
}
