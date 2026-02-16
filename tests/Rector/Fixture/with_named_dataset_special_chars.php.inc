<?php
dataset('user emails (valid)', [
    'test@example.com',
    'user+tag@domain.org',
]);

test('validates email', function (string $email) {
    expect($email)->toContain('@');
})->with('user emails (valid)');
-----
<?php

class WithNamedDatasetSpecialCharsTest extends \PHPUnit\Framework\TestCase
{
    public static function user_emails_valid(): array
    {
        return [
            'test@example.com',
            'user+tag@domain.org',
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('user_emails_valid')]
    public function test_validates_email(string $email): void
    {
        $this->assertContains('@', $email);
    }
}
