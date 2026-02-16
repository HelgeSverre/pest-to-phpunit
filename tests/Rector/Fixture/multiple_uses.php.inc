<?php
uses(Tests\TestCase::class);
uses(Illuminate\Foundation\Testing\RefreshDatabase::class);

test('works', function () {
    expect(true)->toBeTrue();
});
-----
<?php

class MultipleUsesTest extends \Tests\TestCase
{
    use Illuminate\Foundation\Testing\RefreshDatabase;
    public function test_works(): void
    {
        $this->assertTrue(true);
    }
}
