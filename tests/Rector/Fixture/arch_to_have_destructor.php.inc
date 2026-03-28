<?php
arch('has destructor')
    ->expect('App\Resources')
    ->toHaveDestructor();
-----
<?php
class ArchToHaveDestructorTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_has_destructor(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Resources');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->hasMethod('__destruct'), sprintf('Expected %s to have a destructor', $object->name));
        }
    }
}
