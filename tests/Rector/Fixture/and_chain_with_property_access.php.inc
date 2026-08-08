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
        if (is_string($__pest_contain_subject = $user->email)) {
            $this->assertStringContainsString('@', $__pest_contain_subject);
        } else {
            $this->assertContains('@', $__pest_contain_subject);
        }
    }
}
