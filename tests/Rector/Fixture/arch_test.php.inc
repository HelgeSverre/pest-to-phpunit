<?php

arch('app code is strict')
    ->expect('App')
    ->toUseStrictTypes();

test('normal test', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class ArchTest extends \PHPUnit\Framework\TestCase
{
    public function test_app_code_is_strict(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: app code is strict');
    }
    public function test_normal_test(): void
    {
        $this->assertTrue(true);
    }
}
