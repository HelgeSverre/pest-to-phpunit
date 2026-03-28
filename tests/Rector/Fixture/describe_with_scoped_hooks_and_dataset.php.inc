<?php
describe('users', function () {
    beforeEach(function () {
        $this->db = new Database();
    });

    test('can create user', function (string $name) {
        expect($this->db->create($name))->toBeTrue();
    })->with(['Alice', 'Bob']);
});

-----
<?php
class DescribeWithScopedHooksAndDatasetTest extends \PHPUnit\Framework\TestCase
{
    protected $db;
    public static function users_can_create_user_provider(): array
    {
        return [['Alice'], ['Bob']];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('users_can_create_user_provider')]
    public function test_users_can_create_user(string $name): void
    {
        $this->db = new Database();
        $this->assertTrue($this->db->create($name));
    }
}
