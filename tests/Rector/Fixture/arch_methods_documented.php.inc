<?php

arch('documented methods')
    ->expect('App')
    ->toHaveMethodsDocumented();

-----
<?php

class ArchMethodsDocumentedTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_documented_methods(): void
    {
        $this->markTestIncomplete('Arch assertion \'methodsDocumented\' requires manual review');
    }
}
