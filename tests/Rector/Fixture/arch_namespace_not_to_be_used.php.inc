<?php

arch('internal not used')
    ->expect('App\Internal')
    ->not->toBeUsed();

-----
<?php

class ArchNamespaceNotToBeUsedTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_internal_not_used(): void
    {
        $this->markTestIncomplete('Arch assertion \'beUsed\' on namespace requires manual review');
    }
}
