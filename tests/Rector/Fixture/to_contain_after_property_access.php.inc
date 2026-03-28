<?php
test('contains in property value', function () {
    expect($user)->name->toContain('John');
});

-----
<?php
class ToContainAfterPropertyAccessTest extends \PHPUnit\Framework\TestCase
{
    public function test_contains_in_property_value(): void
    {
        $this->assertContains('John', $user->name);
    }
}
