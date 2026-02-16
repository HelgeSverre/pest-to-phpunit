<?php
describe('Workflow', function () {
    test('step one', function () {
        expect(true)->toBeTrue();
    });

    test('step two', function () {
        expect(true)->toBeTrue();
    })->depends('Workflow step one');
});
-----
<?php
class DescribeWithDependsTest extends \PHPUnit\Framework\TestCase
{
    public function test_workflow_step_one(): void
    {
        $this->assertTrue(true);
    }
    #[\PHPUnit\Framework\Attributes\Depends('test_workflow_step_one')]
    public function test_workflow_step_two(): void
    {
        $this->assertTrue(true);
    }
}
