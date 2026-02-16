<?php
test('three and subjects', function () {
    expect($a)->toBeInt()
        ->and($b)->toBeString()
        ->and($c)->toBeArray();
});
-----
<?php
class AndChainThreeSubjectsTest extends \PHPUnit\Framework\TestCase
{
    public function test_three_and_subjects(): void
    {
        $this->assertIsInt($a);
        $this->assertIsString($b);
        $this->assertIsArray($c);
    }
}
