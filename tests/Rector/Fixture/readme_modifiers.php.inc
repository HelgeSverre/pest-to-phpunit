<?php

test('skipped test', function () {
    expect(true)->toBeTrue();
})->skip('Not ready yet');

test('todo test', function () {
})->todo();

test('throws exception', function () {
    throw new RuntimeException('fail');
})->throws(RuntimeException::class, 'fail');

test('grouped test', function () {
    expect(true)->toBeTrue();
})->group('unit');

-----
<?php

class ReadmeModifiersTest extends \PHPUnit\Framework\TestCase
{
    public function test_skipped_test(): void
    {
        $this->markTestSkipped('Not ready yet');
    }
    public function test_todo_test(): void
    {
        $this->markTestIncomplete('TODO');
    }
    public function test_throws_exception(): void
    {
        $this->expectException(RuntimeException::class);
        $this->expectExceptionMessage('fail');
        throw new RuntimeException('fail');
    }
    #[\PHPUnit\Framework\Attributes\Group('unit')]
    public function test_grouped_test(): void
    {
        $this->assertTrue(true);
    }
}