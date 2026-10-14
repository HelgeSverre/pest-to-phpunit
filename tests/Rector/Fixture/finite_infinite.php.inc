<?php
test('finite and infinite', function () {
    expect(1.0)->toBeFinite();
    expect(INF)->toBeInfinite();
    expect([1.0])->each->toBeFinite();
});
-----
<?php
class FiniteInfiniteTest extends \PHPUnit\Framework\TestCase
{
    public function test_finite_and_infinite(): void
    {
        $this->assertFinite(1.0);
        $this->assertInfinite(INF);
        foreach ([1.0] as $__pest_each_item) {
            $this->assertFinite($__pest_each_item);
        }
    }
}
