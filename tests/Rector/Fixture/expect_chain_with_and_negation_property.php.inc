<?php
test('complex chain', function () {
    expect($user)->name->toBeString()
        ->and($user)->age->toBeGreaterThan(0)
        ->and($user)->email->not->toBeEmpty();
});
-----
<?php
class ExpectChainWithAndNegationPropertyTest extends \PHPUnit\Framework\TestCase
{
    public function test_complex_chain(): void
    {
        $this->assertIsString($user->name);
        $this->assertGreaterThan(0, $user->age);
        $this->assertNotEmpty($user->email);
    }
}
