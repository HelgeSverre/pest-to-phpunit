<?php

use App\Models\User;
use App\Services\AuthService;

uses(Tests\TestCase::class, Illuminate\Foundation\Testing\RefreshDatabase::class);

covers(AuthService::class);

beforeEach(function () {
    $this->authService = new AuthService();
    $this->user = User::factory()->create();
});

test('authenticates valid user', function () {
    $result = $this->authService->authenticate(
        $this->user->email,
        'password'
    );

    expect($result)->toBeTrue();
});

test('rejects invalid password', function () {
    $result = $this->authService->authenticate(
        $this->user->email,
        'wrong'
    );

    expect($result)->toBeFalse();
});

test('throws on missing user', function () {
    expect(fn () => $this->authService->authenticate('nobody@test.com', 'pass'))
        ->toThrow(InvalidArgumentException::class, 'User not found');
});
-----
<?php

use App\Models\User;
use App\Services\AuthService;
#[\PHPUnit\Framework\Attributes\CoversClass(AuthService::class)]
class ComplexRealWorldTest extends \Tests\TestCase
{
    use Illuminate\Foundation\Testing\RefreshDatabase;
    protected $authService;
    protected $user;
    protected function setUp(): void
    {
        parent::setUp();
        $this->authService = new AuthService();
        $this->user = User::factory()->create();
    }
    public function test_authenticates_valid_user(): void
    {
        $result = $this->authService->authenticate(
            $this->user->email,
            'password'
        );
        $this->assertTrue($result);
    }
    public function test_rejects_invalid_password(): void
    {
        $result = $this->authService->authenticate(
            $this->user->email,
            'wrong'
        );
        $this->assertFalse($result);
    }
    public function test_throws_on_missing_user(): void
    {
        $this->expectException(InvalidArgumentException::class);
        $this->expectExceptionMessage('User not found');
        (fn () => $this->authService->authenticate('nobody@test.com', 'pass'))();
    }
}
