<?php
arch('app models')
    ->expect('App\Models')
    ->toBeClasses();

arch('strict types')
    ->expect('App')
    ->toUseStrictTypes();
-----
<?php

class ArchWithChainsTest extends \PHPUnit\Framework\TestCase
{
    public function test_app_models(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: app models');
    }
    public function test_strict_types(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: strict types');
    }
}
