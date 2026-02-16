<?php

uses(Tests\TestCase::class);

dataset('valid emails', [
    'simple' => ['user@example.com'],
    'with plus' => ['user+tag@example.com'],
    'subdomain' => ['user@sub.example.com'],
]);

dataset('invalid emails', [
    'no at' => ['userexample.com'],
    'no domain' => ['user@'],
    'empty' => [''],
]);

test('accepts valid emails', function (string $email) {
    expect(filter_var($email, FILTER_VALIDATE_EMAIL))->not->toBeFalse();
})->with('valid emails');

test('rejects invalid emails', function (string $email) {
    expect(filter_var($email, FILTER_VALIDATE_EMAIL))->toBeFalse();
})->with('invalid emails');
-----
<?php

class RealWorldValidationTest extends \Tests\TestCase
{
    public static function valid_emails(): array
    {
        return [
            'simple' => ['user@example.com'],
            'with plus' => ['user+tag@example.com'],
            'subdomain' => ['user@sub.example.com'],
        ];
    }
    public static function invalid_emails(): array
    {
        return [
            'no at' => ['userexample.com'],
            'no domain' => ['user@'],
            'empty' => [''],
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('valid_emails')]
    public function test_accepts_valid_emails(string $email): void
    {
        $this->assertNotFalse(filter_var($email, FILTER_VALIDATE_EMAIL));
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('invalid_emails')]
    public function test_rejects_invalid_emails(string $email): void
    {
        $this->assertFalse(filter_var($email, FILTER_VALIDATE_EMAIL));
    }
}
