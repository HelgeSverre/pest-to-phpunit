<?php
describe('Admin', function () {
    test('can delete', function () {
        expect(true)->toBeTrue();
    })->group('admin');

    test('can edit', function () {
        expect(true)->toBeTrue();
    })->group('admin');
});
-----
<?php
class DescribeWithGroupModifierTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\Group('admin')]
    public function test_admin_can_delete(): void
    {
        $this->assertTrue(true);
    }
    #[\PHPUnit\Framework\Attributes\Group('admin')]
    public function test_admin_can_edit(): void
    {
        $this->assertTrue(true);
    }
}
