<?php
test('has properties with values', function () {
    expect($user)->toHaveProperties(['name' => 'Nuno', 'email' => 'foo@bar.com']);
});

-----
<?php
class ToHavePropertiesAssocTest extends \PHPUnit\Framework\TestCase
{
    public function test_has_properties_with_values(): void
    {
        $this->assertSame('Nuno', $user->name);
        $this->assertSame('foo@bar.com', $user->email);
    }
}
