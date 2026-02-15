<?php

uses(Tests\TestCase::class, RefreshDatabase::class);

test('database works', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class UsesTraitsTest extends \Tests\TestCase
{
    use RefreshDatabase;
    public function test_database_works(): void
    {
        $this->assertTrue(true);
    }
}
