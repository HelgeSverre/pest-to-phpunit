<?php
test('matches constraint', function () {
    expect($value)->toMatchConstraint(new \PHPUnit\Framework\Constraint\IsTrue());
});
-----
<?php
class ToMatchConstraintTest extends \PHPUnit\Framework\TestCase
{
    public function test_matches_constraint(): void
    {
        $this->assertThat($value, new \PHPUnit\Framework\Constraint\IsTrue());
    }
}
