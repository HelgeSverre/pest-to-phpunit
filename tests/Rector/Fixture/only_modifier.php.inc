<?php
test('important test', function () {
    expect(true)->toBeTrue();
})->only();

it('also important', function () {
    expect(1 + 1)->toBe(2);
})->only();

-----
<?php

class OnlyModifierTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\Group('only')]
    public function test_important_test(): void
    {
        $this->assertTrue(true);
    }
    #[\PHPUnit\Framework\Attributes\Group('only')]
    public function test_it_also_important(): void
    {
        $this->assertSame(2, 1 + 1);
    }
}
