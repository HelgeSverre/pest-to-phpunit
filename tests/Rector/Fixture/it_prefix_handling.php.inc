<?php
it('it should work', function () {
    expect(true)->toBeTrue();
});

it('iterates correctly', function () {
    expect(true)->toBeTrue();
});
-----
<?php

class ItPrefixHandlingTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_should_work(): void
    {
        $this->assertTrue(true);
    }
    public function test_it_iterates_correctly(): void
    {
        $this->assertTrue(true);
    }
}
