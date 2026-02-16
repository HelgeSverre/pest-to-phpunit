<?php

use App\Models\User;
use App\Services\UserService;

test('creates user via service', function () {
    $service = new UserService();
    $user = $service->create('John');
    expect($user)->toBeInstanceOf(User::class);
});
-----
<?php

use App\Models\User;
use App\Services\UserService;

class TestWithUseImportTest extends \PHPUnit\Framework\TestCase
{
    public function test_creates_user_via_service(): void
    {
        $service = new UserService();
        $user = $service->create('John');
        $this->assertInstanceOf(User::class, $user);
    }
}
