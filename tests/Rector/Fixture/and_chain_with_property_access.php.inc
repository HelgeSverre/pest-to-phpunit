<?php
test('and with property', function () {
    expect($user)->name->toBe('John')
        ->and($user)->email->toContain('@');
});
-----
<?php
class AndChainWithPropertyAccessTest extends \PHPUnit\Framework\TestCase
{
    public function test_and_with_property(): void
    {
        $this->assertSame('John', $user->name);
        $this->assertContains('@', $user->email);
    }
}
