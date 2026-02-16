<?php
test('comparison subject', function () {
    expect(1 < 2)->toBeTrue();
    expect(1 > 2)->toBeFalse();
    expect(1 === 1)->toBeTrue();
});
-----
<?php
class ExpectWithComparisonSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_comparison_subject(): void
    {
        $this->assertTrue(1 < 2);
        $this->assertFalse(1 > 2);
        $this->assertTrue(1 === 1);
    }
}
