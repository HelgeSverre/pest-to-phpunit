<?php
beforeEach(function () {
    if (true) {
        $this->value = 42;
    }
});

test('has value', function () {
    expect($this->value)->toBe(42);
});

-----
<?php

class HookWithPropertyInIfTest extends \PHPUnit\Framework\TestCase
{
    protected $value;
    protected function setUp(): void
    {
        parent::setUp();
        if (true) {
            $this->value = 42;
        }
    }
    public function test_has_value(): void
    {
        $this->assertSame(42, $this->value);
    }
}
