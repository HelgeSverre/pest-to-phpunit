<?php

test('adds numbers', function () {
    expect(1 + 1)->toBe(2);
});

it('subtracts numbers', function () {
    expect(5 - 3)->toBe(2);
});

-----
<?php

class ReadmeBasicTest extends \PHPUnit\Framework\TestCase
{
    public function test_adds_numbers(): void
    {
        $this->assertSame(2, 1 + 1);
    }
    public function test_it_subtracts_numbers(): void
    {
        $this->assertSame(2, 5 - 3);
    }
}