<?php
describe('UserService', function () {
    test('creates user', function () {
        expect(true)->toBeTrue();
    })->covers(App\Services\UserService::class);
});
-----
<?php
class DescribeWithCoversTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\CoversClass(App\Services\UserService::class)]
    public function test_userservice_creates_user(): void
    {
        $this->assertTrue(true);
    }
}
