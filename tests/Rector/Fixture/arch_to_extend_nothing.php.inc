<?php
arch('value objects extend nothing')
    ->expect('App\ValueObjects')
    ->toExtendNothing();
-----
<?php
class ArchToExtendNothingTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_value_objects_extend_nothing(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\ValueObjects');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->getParentClass() === false, sprintf('Expected %s to extend nothing', $object->name));
        }
    }
}
