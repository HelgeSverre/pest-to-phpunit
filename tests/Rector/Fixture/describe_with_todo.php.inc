<?php
describe('Future', function () {
    test('planned feature', function () {
    })->todo();

    test('another planned', function () {
    })->todo();
});
-----
<?php
class DescribeWithTodoTest extends \PHPUnit\Framework\TestCase
{
    public function test_future_planned_feature(): void
    {
        $this->markTestIncomplete('TODO');
    }
    public function test_future_another_planned(): void
    {
        $this->markTestIncomplete('TODO');
    }
}
