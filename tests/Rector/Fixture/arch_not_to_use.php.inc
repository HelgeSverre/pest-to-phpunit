<?php
arch('no forbidden dependencies')
    ->expect('App\Models')
    ->not->toUse('App\Http\Controllers');
-----
<?php
class ArchNotToUseTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_no_forbidden_dependencies(): void
    {
        $targetLayer = $this->layer()->leaveByNameStart('App\Models');
        $depLayer = $this->layer()->leaveByNameStart('App\Http\Controllers');
        $this->assertDoesNotDependOn($targetLayer, $depLayer);
    }
}
