<?php
test('does not match constraint', function () {
    expect($value)->not->toMatchConstraint(new \PHPUnit\Framework\Constraint\IsTrue());
});
-----
<?php
class NotToMatchConstraintTest extends \PHPUnit\Framework\TestCase
{
    public function test_does_not_match_constraint(): void
    {
        $this->assertThat($value, new \PHPUnit\Framework\Constraint\LogicalNot(new \PHPUnit\Framework\Constraint\IsTrue()));
    }
}
