<?php
arch('repositories implement interface')
    ->expect('App\Repositories')
    ->toImplement('App\Contracts\Repository');
-----
<?php
class ArchToImplementTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_repositories_implement_interface(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Repositories');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->implementsInterface('App\Contracts\Repository'), sprintf('Expected %s to implement the given interface', $object->name));
        }
    }
}
