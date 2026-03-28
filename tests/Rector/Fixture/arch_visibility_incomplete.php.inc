<?php
arch('no public methods')
    ->expect('App\ValueObjects')
    ->not->toHavePublicMethods();
-----
<?php
class ArchVisibilityIncompleteTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_no_public_methods(): void
    {
        $this->markTestIncomplete('Arch assertion \'hasPublicMethods\' requires manual review');
    }
}
