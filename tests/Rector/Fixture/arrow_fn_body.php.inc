<?php
it('is true', fn () => expect(true)->toBeTrue());

it('equals two', fn () => expect(1 + 1)->toBe(2));

-----
<?php

class ArrowFnBodyTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_is_true(): void
    {
        $this->assertTrue(true);
    }
    public function test_it_equals_two(): void
    {
        $this->assertSame(2, 1 + 1);
    }
}
