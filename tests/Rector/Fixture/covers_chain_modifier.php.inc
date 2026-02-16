<?php
test('user name', function () {
    expect(true)->toBeTrue();
})->covers(App\Models\User::class);

-----
<?php
class CoversChainModifierTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\CoversClass(App\Models\User::class)]
    public function test_user_name(): void
    {
        $this->assertTrue(true);
    }
}

