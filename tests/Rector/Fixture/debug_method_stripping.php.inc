<?php
test('strips debug methods', function () {
    expect($value)->dd()->toBe(42);
});

test('strips ddWhen', function () {
    expect($value)->ddWhen(true)->toBe(42);
});

test('strips ddUnless', function () {
    expect($value)->ddUnless(false)->toBe(42);
});

test('strips ray', function () {
    expect($value)->ray()->toBeTrue();
});

-----
<?php

class DebugMethodStrippingTest extends \PHPUnit\Framework\TestCase
{
    public function test_strips_debug_methods(): void
    {
        $this->assertSame(42, $value);
    }
    public function test_strips_ddwhen(): void
    {
        $this->assertSame(42, $value);
    }
    public function test_strips_ddunless(): void
    {
        $this->assertSame(42, $value);
    }
    public function test_strips_ray(): void
    {
        $this->assertTrue($value);
    }
}