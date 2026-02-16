<?php

expect()->extend('toBeValidEmail', function () {
    return $this->toMatch('/^[^@]+@[^@]+$/');
});

test('email is valid', function () {
    expect('test@example.com')->toBeValidEmail();
});

-----
<?php

/**
 * TODO(Pest): The following custom expectations were defined via expect()->extend()
 * and cannot be auto-converted to PHPUnit:
 *   - toBeValidEmail
 */
class ExpectExtendTest extends \PHPUnit\Framework\TestCase
{
    // TODO(Pest): Custom expectation 'toBeValidEmail' defined via expect()->extend() cannot be auto-converted to PHPUnit
    public function test_email_is_valid(): void
    {
        // TODO(Pest): Unknown expectation ->toBeValidEmail() has no PHPUnit equivalent
    }
}
