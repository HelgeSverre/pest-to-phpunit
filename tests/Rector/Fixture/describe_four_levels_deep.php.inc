<?php
describe('L1', function () {
    describe('L2', function () {
        describe('L3', function () {
            describe('L4', function () {
                it('deepest test', function () {
                    expect(true)->toBeTrue();
                });
            });
        });
    });
});
-----
<?php
class DescribeFourLevelsDeepTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_l1_l2_l3_l4_deepest_test(): void
    {
        $this->assertTrue(true);
    }
}
