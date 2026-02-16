<?php
test('has properties with values', function () {
    expect($user)->toHaveProperties(['name' => 'John', 'age' => 30]);
});
-----
<?php
class ToHavePropertiesAssociativeTest extends \PHPUnit\Framework\TestCase
{
    public function test_has_properties_with_values(): void
    {
        $this->assertSame('John', $user->name);
        $this->assertSame(30, $user->age);
    }
}
