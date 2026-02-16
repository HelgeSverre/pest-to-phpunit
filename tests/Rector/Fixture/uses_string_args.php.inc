<?php
uses('Tests\TestCase', 'Illuminate\Foundation\Testing\RefreshDatabase');

test('works', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class UsesStringArgsTest extends \Tests\TestCase
{
    use Illuminate\Foundation\Testing\RefreshDatabase;
    public function test_works(): void
    {
        $this->assertTrue(true);
    }
}
