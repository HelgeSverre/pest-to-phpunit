<?php
arch('contracts are interfaces')
    ->expect('App\Contracts')
    ->toBeInterfaces();
-----
<?php
class ArchToBeInterfacesTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_contracts_are_interfaces(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Contracts');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isInterface(), sprintf('Expected %s to be an interface', $object->name));
        }
    }
}
