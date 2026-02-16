<?php
test('skips when condition is true', function () {
    expect(true)->toBeTrue();
})->skip(PHP_OS === 'Windows', 'Not supported on Windows');

test('skips with closure', function () {
    expect(1)->toBe(1);
})->skip(fn () => !extension_loaded('redis'), 'Redis not available');

test('skips with variable', function () {
    expect(true)->toBeTrue();
})->skip($shouldSkip);

-----
<?php

class SkipConditionalTest extends \PHPUnit\Framework\TestCase
{
    public function test_skips_when_condition_is_true(): void
    {
        if (PHP_OS === 'Windows') {
            $this->markTestSkipped('Not supported on Windows');
        }
        $this->assertTrue(true);
    }
    public function test_skips_with_closure(): void
    {
        if ((fn () => !extension_loaded('redis'))()) {
            $this->markTestSkipped('Redis not available');
        }
        $this->assertSame(1, 1);
    }
    public function test_skips_with_variable(): void
    {
        if ($shouldSkip) {
            $this->markTestSkipped('Skipped');
        }
        $this->assertTrue(true);
    }
}
