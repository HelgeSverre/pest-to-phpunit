<?php

arch('empty expect')
    ->expect()
    ->toBeFinal();

-----
<?php

class ArchExpectNoArgsTest extends \PHPUnit\Framework\TestCase
{
    public function test_empty_expect(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: empty expect');
    }
}
