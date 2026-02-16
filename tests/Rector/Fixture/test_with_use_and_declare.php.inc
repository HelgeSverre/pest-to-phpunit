<?php

declare(strict_types=1);

use App\Models\User;

test('strict with imports', function () {
    expect(new User())->toBeInstanceOf(User::class);
});
-----
<?php

declare(strict_types=1);

use App\Models\User;

class TestWithUseAndDeclareTest extends \PHPUnit\Framework\TestCase
{
    public function test_strict_with_imports(): void
    {
        $this->assertInstanceOf(User::class, new User());
    }
}
