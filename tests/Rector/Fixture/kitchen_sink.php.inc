<?php

declare(strict_types=1);

use App\Models\User;
use App\Services\UserService;

uses(Tests\TestCase::class, Illuminate\Foundation\Testing\RefreshDatabase::class);

covers(UserService::class);

dataset('roles', ['admin', 'editor', 'viewer']);

beforeEach(function () {
    $this->service = new UserService();
    $this->admin = User::factory()->create(['role' => 'admin']);
});

afterEach(function () {
    User::query()->delete();
});

test('creates user with role', function (string $role) {
    $user = $this->service->create('Test User', $role);

    expect($user)->toBeInstanceOf(User::class)
        ->and($user->name)->toBe('Test User')
        ->and($user->role)->toBe($role);
})->with('roles');

test('prevents duplicate emails', function () {
    expect(fn () => $this->service->create('Test', 'admin', $this->admin->email))
        ->toThrow(\Illuminate\Database\UniqueConstraintViolationException::class);
})->group('validation');

describe('permissions', function () {
    test('admin can manage users', function () {
        $result = $this->service->checkPermission($this->admin, 'manage_users');
        expect($result)->toBeTrue();
    });

    test('viewer cannot manage users', function () {
        $viewer = User::factory()->create(['role' => 'viewer']);
        $result = $this->service->checkPermission($viewer, 'manage_users');
        expect($result)->toBeFalse();
    });

    test('unknown permission returns false', function () {
        expect($this->service->checkPermission($this->admin, 'fly'))->toBeFalse();
    });
});

test('soft deletes user', function () {
    $user = User::factory()->create();
    $this->service->delete($user);

    expect(User::find($user->id))->toBeNull();
    expect(User::withTrashed()->find($user->id))->not->toBeNull();
});

test('lists active users', function () {
    User::factory()->count(3)->create(['active' => true]);
    User::factory()->count(2)->create(['active' => false]);

    $active = $this->service->listActive();

    expect($active)->toHaveCount(4);
    expect($active)->each->toBeInstanceOf(User::class);
})->skip('Flaky on CI');
-----
<?php

declare(strict_types=1);

use App\Models\User;
use App\Services\UserService;
#[\PHPUnit\Framework\Attributes\CoversClass(UserService::class)]
class KitchenSinkTest extends \Tests\TestCase
{
    use Illuminate\Foundation\Testing\RefreshDatabase;
    protected $service;
    protected $admin;
    public static function roles(): array
    {
        return ['admin', 'editor', 'viewer'];
    }
    protected function setUp(): void
    {
        parent::setUp();
        $this->service = new UserService();
        $this->admin = User::factory()->create(['role' => 'admin']);
    }
    protected function tearDown(): void
    {
        parent::tearDown();
        User::query()->delete();
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('roles')]
    public function test_creates_user_with_role(string $role): void
    {
        $user = $this->service->create('Test User', $role);
        $this->assertInstanceOf(User::class, $user);
        $this->assertSame('Test User', $user->name);
        $this->assertSame($role, $user->role);
    }
    #[\PHPUnit\Framework\Attributes\Group('validation')]
    public function test_prevents_duplicate_emails(): void
    {
        $this->expectException(\Illuminate\Database\UniqueConstraintViolationException::class);
        (fn () => $this->service->create('Test', 'admin', $this->admin->email))();
    }
    public function test_permissions_admin_can_manage_users(): void
    {
        $result = $this->service->checkPermission($this->admin, 'manage_users');
        $this->assertTrue($result);
    }
    public function test_permissions_viewer_cannot_manage_users(): void
    {
        $viewer = User::factory()->create(['role' => 'viewer']);
        $result = $this->service->checkPermission($viewer, 'manage_users');
        $this->assertFalse($result);
    }
    public function test_permissions_unknown_permission_returns_false(): void
    {
        $this->assertFalse($this->service->checkPermission($this->admin, 'fly'));
    }
    public function test_soft_deletes_user(): void
    {
        $user = User::factory()->create();
        $this->service->delete($user);
        $this->assertNull(User::find($user->id));
        $this->assertNotNull(User::withTrashed()->find($user->id));
    }
    public function test_lists_active_users(): void
    {
        $this->markTestSkipped('Flaky on CI');
    }
}
