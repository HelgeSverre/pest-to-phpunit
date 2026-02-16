<?php
test('and chain with not', function () {
    expect($a)->toBe(1)
        ->and($b)->not->toBe(2);
});

-----
<?php
class AndChainWithNegationTest extends \PHPUnit\Framework\TestCase
{
    public function test_and_chain_with_not(): void
    {
        $this->assertSame(1, $a);
        $this->assertNotSame(2, $b);
    }
}
