<?php

arch('extends nothing arg')
    ->expect('App\Models')
    ->toExtend();

-----
<?php

class ArchToExtendNoArgsTest extends \PHPUnit\Framework\TestCase
{
    public function test_extends_nothing_arg(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: extends nothing arg');
    }
}
