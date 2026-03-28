<?php

arch('models only use value objects')
    ->expect('App\Models')
    ->toOnlyUse('App\ValueObjects');

-----
<?php

class ArchToOnlyUseTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_only_use_value_objects(): void
    {
        $this->markTestIncomplete('Arch assertion \'onlyDependOn\' requires manual review');
    }
}
