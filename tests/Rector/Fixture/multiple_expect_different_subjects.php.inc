<?php
test('multiple expects', function () {
    expect($user->name)->toBe('John');
    expect($user->email)->toContain('@');
    expect($user->age)->toBeGreaterThan(18);
    expect($user->active)->toBeTrue();
    expect($user->roles)->toBeArray();
    expect($user->roles)->toHaveCount(2);
});
-----
<?php
class MultipleExpectDifferentSubjectsTest extends \PHPUnit\Framework\TestCase
{
    public function test_multiple_expects(): void
    {
        $this->assertSame('John', $user->name);
        $this->assertContains('@', $user->email);
        $this->assertGreaterThan(18, $user->age);
        $this->assertTrue($user->active);
        $this->assertIsArray($user->roles);
        $this->assertCount(2, $user->roles);
    }
}
