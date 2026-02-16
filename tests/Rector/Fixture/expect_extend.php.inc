<?php

expect()->extend('toBeValidEmail', function () {
    return $this->toMatch('/^[^@]+@[^@]+$/');
});

test('email is valid', function () {
    expect('test@example.com')->toBeValidEmail();
});

-----
<?php

class ExpectExtendTest extends \PHPUnit\Framework\TestCase
{
    public function test_email_is_valid(): void
    {
        $this->assertMatchesRegularExpression('/^[^@]+@[^@]+$/', 'test@example.com');
    }
}
