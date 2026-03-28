<?php
arch('base classes are abstract')
    ->expect('App\Base')
    ->toBeAbstract();
-----
<?php
class ArchToBeAbstractTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_base_classes_are_abstract(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Base');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isAbstract(), sprintf('Expected %s to be abstract', $object->name));
        }
    }
}
