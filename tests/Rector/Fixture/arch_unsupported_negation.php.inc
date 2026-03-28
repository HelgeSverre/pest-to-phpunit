<?php
arch('negated line count')
    ->expect('App')
    ->not->toHaveLineCountLessThan(500);
-----
<?php
class ArchUnsupportedNegationTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_line_count(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: negated line count');
    }
}
