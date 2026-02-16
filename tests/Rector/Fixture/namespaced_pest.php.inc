<?php

namespace Tests\Feature;

use App\Models\User;

test('creates user', function () {
    $user = new User();
    expect($user)->toBeInstanceOf(User::class);
});

-----
<?php

namespace Tests\Feature;

use App\Models\User;

class NamespacedPestTest extends \PHPUnit\Framework\TestCase
{
    public function test_creates_user(): void
    {
        $user = new User();
        $this->assertInstanceOf(User::class, $user);
    }
}
