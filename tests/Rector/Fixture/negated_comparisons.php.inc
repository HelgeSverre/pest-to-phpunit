<?php
test('negated comparisons', function () {
    expect($x)->not->toBeGreaterThan(5);
    expect($x)->not->toBeGreaterThanOrEqual(5);
    expect($x)->not->toBeLessThan(5);
    expect($x)->not->toBeLessThanOrEqual(5);
});
-----
<?php
class NegatedComparisonsTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_comparisons(): void
    {
        $this->assertLessThanOrEqual(5, $x);
        $this->assertLessThan(5, $x);
        $this->assertGreaterThanOrEqual(5, $x);
        $this->assertGreaterThan(5, $x);
    }
}
