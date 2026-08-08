<?php
test('negated finite and infinite', function () {
    expect(NAN)->not->toBeFinite();
    expect(NAN)->not->toBeInfinite();
    expect([NAN])->each->not->toBeFinite();
});
-----
<?php
class NegatedFiniteInfiniteTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_finite_and_infinite(): void
    {
        $this->assertFalse(is_finite(NAN));
        $this->assertFalse(is_infinite(NAN));
        foreach ([NAN] as $__pest_each_item) {
            $this->assertFalse(is_finite($__pest_each_item));
        }
    }
}
