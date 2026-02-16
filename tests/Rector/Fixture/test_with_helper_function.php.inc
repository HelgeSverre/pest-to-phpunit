<?php

function createTestUser(string $name = 'John'): array
{
    return ['name' => $name, 'active' => true];
}

test('uses helper', function () {
    $user = createTestUser();
    expect($user['name'])->toBe('John');
});

test('helper with arg', function () {
    $user = createTestUser('Jane');
    expect($user['name'])->toBe('Jane');
});
-----
<?php

function createTestUser(string $name = 'John'): array
{
    return ['name' => $name, 'active' => true];
}
class TestWithHelperFunctionTest extends \PHPUnit\Framework\TestCase
{
    public function test_uses_helper(): void
    {
        $user = createTestUser();
        $this->assertSame('John', $user['name']);
    }
    public function test_helper_with_arg(): void
    {
        $user = createTestUser('Jane');
        $this->assertSame('Jane', $user['name']);
    }
}
