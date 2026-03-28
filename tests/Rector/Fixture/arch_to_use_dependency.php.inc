<?php
arch('services use repositories')
    ->expect('App\Services')
    ->toUse('App\Repositories');
-----
<?php
class ArchToUseDependencyTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_services_use_repositories(): void
    {
        $targetLayer = $this->layer()->leaveByNameStart('App\Services');
        $depLayer = $this->layer()->leaveByNameStart('App\Repositories');
        $this->assertDependOn($targetLayer, $depLayer);
    }
}
