<?php
test('not equal canonicalizing', function () {
    expect([1, 2])->not->toEqualCanonicalizing([3, 4]);
});
-----
<?php
class NegatedEqualCanonicalizingTest extends \PHPUnit\Framework\TestCase
{
    public function test_not_equal_canonicalizing(): void
    {
        $this->assertNotEqualsCanonicalizing([3, 4], [1, 2]);
    }
}
