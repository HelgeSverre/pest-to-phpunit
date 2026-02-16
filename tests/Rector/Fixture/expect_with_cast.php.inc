<?php
test('cast subject', function () {
    expect((int) '42')->toBe(42);
    expect((string) 123)->toBe('123');
    expect((array) $obj)->toBeArray();
});
-----
<?php
class ExpectWithCastTest extends \PHPUnit\Framework\TestCase
{
    public function test_cast_subject(): void
    {
        $this->assertSame(42, (int) '42');
        $this->assertSame('123', (string) 123);
        $this->assertIsArray((array) $obj);
    }
}
