<?php
arch('value objects are readonly')
    ->expect('App\ValueObjects')
    ->toBeReadonly();
-----
<?php
class ArchToBeReadonlyTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_value_objects_are_readonly(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\ValueObjects');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isReadOnly(), sprintf('Expected %s to be readonly', $object->name));
        }
    }
}
