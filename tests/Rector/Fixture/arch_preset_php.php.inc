<?php

arch('php preset')
    ->preset()
    ->php();

-----
<?php

class ArchPresetPhpTest extends \PHPUnit\Framework\TestCase
{
    public function test_php_preset(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: php preset');
    }
}
