<?php

namespace Tests\Feature;

use App\Models\User;

uses(\Tests\TestCase::class);

test('user feature', function () {
    expect(new User())->toBeInstanceOf(User::class);
});
-----
<?php

namespace Tests\Feature;

use App\Models\User;
class NamespacedWithUsesTest extends \Tests\TestCase
{
    public function test_user_feature(): void
    {
        $this->assertInstanceOf(User::class, new User());
    }
}
