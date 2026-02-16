<?php
test('creates user', function () {
    expect(true)->toBeTrue();
});

it('validates email', function () {
    expect('test@example.com')->toContain('@');
});

test('deletes user', function () {
    expect(true)->toBeTrue();
});

it('sends notification', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class MixedTestAndItTest extends \PHPUnit\Framework\TestCase
{
    public function test_creates_user(): void
    {
        $this->assertTrue(true);
    }
    public function test_it_validates_email(): void
    {
        $this->assertContains('@', 'test@example.com');
    }
    public function test_deletes_user(): void
    {
        $this->assertTrue(true);
    }
    public function test_it_sends_notification(): void
    {
        $this->assertTrue(true);
    }
}
