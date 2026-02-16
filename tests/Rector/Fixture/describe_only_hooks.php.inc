<?php
describe('Setup only', function () {
    test('placeholder', function () {
        expect(true)->toBeTrue();
    });
});
-----
<?php
class DescribeOnlyHooksTest extends \PHPUnit\Framework\TestCase
{
    public function test_setup_only_placeholder(): void
    {
        $this->assertTrue(true);
    }
}
