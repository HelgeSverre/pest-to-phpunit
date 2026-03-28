<?php

arch('contracts')
    ->expect('App\Contracts')
    ->interfaces()
    ->toBeInterfaces();

-----
<?php

class ArchInterfacesFilterTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_contracts(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Contracts')->leaveByType(\PHPUnit\Architecture\Enums\ObjectType::_INTERFACE);
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isInterface(), sprintf('Expected %s to be an interface', $object->name));
        }
    }
}
