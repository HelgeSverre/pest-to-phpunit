<?php

beforeEach(function () {
    $this->user = new User();
    $this->service = new UserService();
});

test('user has service', function () {
    expect($this->service)->not->toBeNull();
    $this->user->name = 'test';
    expect($this->user->name)->toBe('test');
});

-----
<?php

class PropertyDeclarationsTest extends \PHPUnit\Framework\TestCase
{
    protected $user;
    protected $service;
    protected function setUp(): void
    {
        parent::setUp();
        $this->user = new User();
        $this->service = new UserService();
    }
    public function test_user_has_service(): void
    {
        $this->assertNotNull($this->service);
        $this->user->name = 'test';
        $this->assertSame('test', $this->user->name);
    }
}
