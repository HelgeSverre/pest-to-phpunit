<?php
test('step one', function () {
    expect(true)->toBeTrue();
});

test('step two', function () {
    expect(true)->toBeTrue();
});

test('step three', function () {
    expect(true)->toBeTrue();
})->depends('step one', 'step two');
-----
<?php

class MultipleDependsTest extends \PHPUnit\Framework\TestCase
{
    public function test_step_one(): void
    {
        $this->assertTrue(true);
    }
    public function test_step_two(): void
    {
        $this->assertTrue(true);
    }
    #[\PHPUnit\Framework\Attributes\Depends('test_step_one')]
    #[\PHPUnit\Framework\Attributes\Depends('test_step_two')]
    public function test_step_three(): void
    {
        $this->assertTrue(true);
    }
}
