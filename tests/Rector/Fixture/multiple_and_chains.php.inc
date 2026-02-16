<?php
test('multiple and chains', function () {
    expect($name)->toBeString()
        ->and($age)->toBeInt()
        ->and($active)->toBeTrue();
});

-----
<?php
class MultipleAndChainsTest extends \PHPUnit\Framework\TestCase
{
    public function test_multiple_and_chains(): void
    {
        $this->assertIsString($name);
        $this->assertIsInt($age);
        $this->assertTrue($active);
    }
}
