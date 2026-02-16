<?php

describe('User', function () {
    beforeEach(function () {
        $this->user = createUser();
    });

    test('has name', function () {
        expect($this->user->name)->toBeString();
    });

    test('has email', function () {
        expect($this->user->email)->toContain('@');
    });
});

-----
<?php

class DescribeScopedBeforeEachTest extends \PHPUnit\Framework\TestCase
{
    protected $user;
    public function test_user_has_name(): void
    {
        $this->user = createUser();
        $this->assertIsString($this->user->name);
    }
    public function test_user_has_email(): void
    {
        $this->user = createUser();
        $this->assertContains('@', $this->user->email);
    }
}
