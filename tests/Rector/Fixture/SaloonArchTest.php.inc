<?php

it('can test', fn () => expect(true)->toBeTrue());

it('will not use debugging functions')
    ->expect(['dd', 'dump', 'ray'])
    ->each->not->toBeUsed();

-----
<?php

class SaloonArchTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_can_test(): void
    {
        $this->assertTrue(true);
    }
    public function test_it_will_not_use_debugging_functions(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: will not use debugging functions');
    }
}
