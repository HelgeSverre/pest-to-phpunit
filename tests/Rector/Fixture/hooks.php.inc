<?php

beforeEach(function () {
    $this->user = new User();
});

afterEach(function () {
    $this->user = null;
});

test('user exists', function () {
    expect($this->user)->not->toBeNull();
});

-----
<?php

class HooksTest extends \PHPUnit\Framework\TestCase
{
    protected function setUp(): void
    {
        parent::setUp();
        $this->user = new User();
    }
    protected function tearDown(): void
    {
        parent::tearDown();
        $this->user = null;
    }
    public function test_user_exists(): void
    {
        $this->assertNotNull($this->user);
    }
}
