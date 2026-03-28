<?php

arch('repos used in services')
    ->expect('App\Repositories')
    ->toBeUsedIn('App\Services');

-----
<?php

class ArchToBeUsedInTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_repos_used_in_services(): void
    {
        $targetLayer = $this->layer()->leaveByNameStart('App\Repositories');
        $depLayer = $this->layer()->leaveByNameStart('App\Services');
        $this->assertDependOn($depLayer, $targetLayer);
    }
}
