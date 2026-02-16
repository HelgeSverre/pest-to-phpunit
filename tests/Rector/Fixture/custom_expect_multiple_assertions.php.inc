<?php
expect()->extend('toBeValidEmail', function () {
    return $this->toBeString()
        ->toContain('@')
        ->toMatch('/^[^@]+@[^@]+\.[^@]+$/');
});

test('valid email', function () {
    expect('user@example.com')->toBeValidEmail();
});

-----
<?php

class CustomExpectMultipleAssertionsTest extends \PHPUnit\Framework\TestCase
{
    public function test_valid_email(): void
    {
        $this->assertIsString('user@example.com');
        $this->assertStringContainsString('@', 'user@example.com');
        $this->assertMatchesRegularExpression('/^[^@]+@[^@]+\.[^@]+$/', 'user@example.com');
    }
}
