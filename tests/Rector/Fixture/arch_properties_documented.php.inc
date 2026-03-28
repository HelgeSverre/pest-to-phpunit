<?php

arch('documented properties')
    ->expect('App')
    ->toHavePropertiesDocumented();

-----
<?php

class ArchPropertiesDocumentedTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_documented_properties(): void
    {
        $this->markTestIncomplete('Arch assertion \'propertiesDocumented\' requires manual review');
    }
}
