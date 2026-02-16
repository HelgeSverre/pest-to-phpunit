<?php

uses(Tests\TestCase::class)->in('Feature');

test('it runs', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class UsesWithInTest extends \Tests\TestCase
{
    public function test_it_runs(): void
    {
        $this->assertTrue(true);
    }
}
