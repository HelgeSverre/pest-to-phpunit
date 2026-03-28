<?php
arch('only classes are final')
    ->expect('App\Http')
    ->classes()
    ->toBeFinal();
-----
<?php
class ArchWithClassesFilterTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_only_classes_are_final(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Http')->leaveByType(\PHPUnit\Architecture\Enums\ObjectType::_CLASS);
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isFinal(), sprintf('Expected %s to be final', $object->name));
        }
    }
}
