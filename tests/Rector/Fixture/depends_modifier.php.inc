<?php
test('creates user', function () {
    expect(true)->toBeTrue();
});

test('updates user', function () {
    expect(true)->toBeTrue();
})->depends('creates user');

-----
<?php

class DependsModifierTest extends \PHPUnit\Framework\TestCase
{
    public function test_creates_user(): void
    {
        $this->assertTrue(true);
    }
    #[\PHPUnit\Framework\Attributes\Depends('test_creates_user')]
    public function test_updates_user(): void
    {
        $this->assertTrue(true);
    }
}
