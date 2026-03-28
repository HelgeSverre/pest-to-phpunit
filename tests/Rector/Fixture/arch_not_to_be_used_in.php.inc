<?php

arch('models not used in controllers')
    ->expect('App\Models')
    ->not->toBeUsedIn('App\Http\Controllers');

-----
<?php

class ArchNotToBeUsedInTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_not_used_in_controllers(): void
    {
        $targetLayer = $this->layer()->leaveByNameStart('App\Models');
        $depLayer = $this->layer()->leaveByNameStart('App\Http\Controllers');
        $this->assertDoesNotDependOn($depLayer, $targetLayer);
    }
}
