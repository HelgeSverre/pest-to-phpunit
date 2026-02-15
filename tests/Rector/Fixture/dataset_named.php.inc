<?php

dataset('emails', [
    'test@example.com',
    'foo@bar.com',
]);

test('validates email', function (string $email) {
    expect($email)->toContain('@');
})->with('emails');

-----
<?php

class DatasetNamedTest extends \PHPUnit\Framework\TestCase
{
    public static function emails(): array
    {
        return [
            'test@example.com',
            'foo@bar.com',
        ];
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('emails')]
    public function test_validates_email(string $email): void
    {
        $this->assertContains('@', $email);
    }
}
