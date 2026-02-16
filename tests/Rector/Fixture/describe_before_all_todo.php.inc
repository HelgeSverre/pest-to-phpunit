<?php
describe('Database', function () {
    beforeAll(function () {
        DB::seed();
    });

    afterAll(function () {
        DB::truncate();
    });

    test('has data', function () {
        expect(true)->toBeTrue();
    });
});
-----
<?php
class DescribeBeforeAllTodoTest extends \PHPUnit\Framework\TestCase
{
    public function test_database_has_data(): void
    {
        // TODO(Pest): beforeAll() inside describe() cannot be scoped in PHPUnit — move to setUpBeforeClass() or inline
        // TODO(Pest): afterAll() inside describe() cannot be scoped in PHPUnit — move to tearDownAfterClass() or inline
        $this->assertTrue(true);
    }
}
