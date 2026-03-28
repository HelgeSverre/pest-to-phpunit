<?php
arch('missing expect');
-----
<?php
class ArchNoExpectTest extends \PHPUnit\Framework\TestCase
{
    public function test_missing_expect(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: missing expect');
    }
}
