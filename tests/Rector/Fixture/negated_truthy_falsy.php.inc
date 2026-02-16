<?php
test('negated truthy falsy', function () {
    expect(0)->not->toBeTruthy();
    expect(1)->not->toBeFalsy();
});
-----
<?php
class NegatedTruthyFalsyTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_truthy_falsy(): void
    {
        $this->assertEmpty(0);
        $this->assertNotEmpty(1);
    }
}
