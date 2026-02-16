<?php
test('null subject', function () {
    expect(null)->toBeNull();
    expect(null)->not->toBeTrue();
    expect(null)->toBeFalsy();
});
-----
<?php
class ExpectWithNullSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_null_subject(): void
    {
        $this->assertNull(null);
        $this->assertNotTrue(null);
        $this->assertEmpty(null);
    }
}
