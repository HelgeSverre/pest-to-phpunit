<?php
test('empty array subject', function () {
    expect([])->toBeEmpty();
    expect([])->toBeArray();
    expect([])->toHaveCount(0);
    expect([])->toBeList();
});
-----
<?php
class ExpectWithEmptyArraySubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_empty_array_subject(): void
    {
        $this->assertEmpty([]);
        $this->assertIsArray([]);
        $this->assertCount(0, []);
        $this->assertIsList([]);
    }
}
