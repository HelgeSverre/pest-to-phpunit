<?php

arch('classes are final except legacy')
    ->expect('App')
    ->classes()
    ->ignoring('App\Legacy')
    ->toBeFinal();

-----
<?php

class ArchFilterWithIgnoringTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_classes_are_final_except_legacy(): void
    {
        $layer = $this->layer()->leaveByNameStart('App')->leaveByType(\PHPUnit\Architecture\Enums\ObjectType::_CLASS)->excludeByNameStart('App\Legacy');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isFinal(), sprintf('Expected %s to be final', $object->name));
        }
    }
}
