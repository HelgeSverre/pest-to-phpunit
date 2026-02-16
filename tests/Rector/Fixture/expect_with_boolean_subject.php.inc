<?php
test('boolean subjects', function () {
    expect(true)->toBeTrue();
    expect(false)->toBeFalse();
    expect(true)->toBeTruthy();
    expect(false)->toBeFalsy();
    expect(true)->toBeBool();
    expect(false)->toBeBool();
});
-----
<?php
class ExpectWithBooleanSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_boolean_subjects(): void
    {
        $this->assertTrue(true);
        $this->assertFalse(false);
        $this->assertNotEmpty(true);
        $this->assertEmpty(false);
        $this->assertIsBool(true);
        $this->assertIsBool(false);
    }
}
