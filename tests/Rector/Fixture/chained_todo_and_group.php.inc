<?php
test('todo with group', function () {
})->todo()->group('future');
-----
<?php
class ChainedTodoAndGroupTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\Group('future')]
    public function test_todo_with_group(): void
    {
        $this->markTestIncomplete('TODO');
    }
}
