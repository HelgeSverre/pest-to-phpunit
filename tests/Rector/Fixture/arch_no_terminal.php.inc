<?php
arch('no assertion')
    ->expect('App');
-----
<?php
class ArchNoTerminalTest extends \PHPUnit\Framework\TestCase
{
    public function test_no_assertion(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: no assertion');
    }
}
