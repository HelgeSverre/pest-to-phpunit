<?php
describe('Level 1', function () {
    describe('Level 2', function () {
        describe('Level 3', function () {
            it('works at depth', function () {
                expect(true)->toBeTrue();
            });
        });
    });
});

-----
<?php
class NestedDescribeDeepTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_level_1_level_2_level_3_works_at_depth(): void
    {
        $this->assertTrue(true);
    }
}

