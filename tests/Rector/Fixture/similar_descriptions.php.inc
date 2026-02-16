<?php
test('creates user', function () {
    expect(true)->toBeTrue();
});

test('creates user with email', function () {
    expect(true)->toBeTrue();
});

test('creates user with role', function () {
    expect(true)->toBeTrue();
});
-----
<?php

class SimilarDescriptionsTest extends \PHPUnit\Framework\TestCase
{
    public function test_creates_user(): void
    {
        $this->assertTrue(true);
    }
    public function test_creates_user_with_email(): void
    {
        $this->assertTrue(true);
    }
    public function test_creates_user_with_role(): void
    {
        $this->assertTrue(true);
    }
}
