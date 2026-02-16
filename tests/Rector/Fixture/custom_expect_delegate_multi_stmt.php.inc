<?php
// Bug: delegate body with multiple $this->toX() statements only inlined the first
expect()->extend('toLookLikeEmail', function () {
    $this->toBeString();
    $this->toContain('@');
    return $this;
});

test('email-ish', function () {
    expect('a@b.com')->toLookLikeEmail();
});

-----
<?php

class CustomExpectDelegateMultiStmtTest extends \PHPUnit\Framework\TestCase
{
    public function test_emailish(): void
    {
        $this->assertIsString('a@b.com');
        $this->assertStringContainsString('@', 'a@b.com');
    }
}
