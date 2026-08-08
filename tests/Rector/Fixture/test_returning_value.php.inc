<?php
test('returns value', function () {
    $user = createUser();
    expect($user->name)->toBe('John');
    return $user;
});
-----
<?php
class TestReturningValueTest extends \PHPUnit\Framework\TestCase
{
    public function test_returns_value(): void
    {
        $user = createUser();
        $this->assertSame('John', $user->name);
        // TODO(Pest): test return value was discarded because PHPUnit tests must return void
        return;
    }
}
