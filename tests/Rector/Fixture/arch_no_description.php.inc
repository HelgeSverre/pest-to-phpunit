<?php
arch()->expect('App\Models')->toExtend('Illuminate\Database\Eloquent\Model');
-----
<?php
class ArchNoDescriptionTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_arch_test(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isSubclassOf('Illuminate\Database\Eloquent\Model'), sprintf('Expected %s to extend the given class', $object->name));
        }
    }
}
