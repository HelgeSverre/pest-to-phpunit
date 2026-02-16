<?php

function createUser(): object {
    return (object) ['name' => 'John'];
}

test('uses helper', function () {
    $user = createUser();
    expect($user->name)->toBe('John');
});

-----
<?php

function createUser(): object {
    return (object) ['name' => 'John'];
}

class MixedCodeTest extends \PHPUnit\Framework\TestCase
{
    public function test_uses_helper(): void
    {
        $user = createUser();
        $this->assertSame('John', $user->name);
    }
}
