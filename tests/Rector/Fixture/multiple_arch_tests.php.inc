<?php
arch('models extend base')
    ->expect('App\Models')
    ->toExtend('Illuminate\Database\Eloquent\Model');

arch('controllers are final')
    ->expect('App\Http\Controllers')
    ->toBeFinal();

arch('no debugging')
    ->expect(['dd', 'dump', 'ray'])
    ->not->toBeUsed();
-----
<?php

class MultipleArchTestsTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_extend_base(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isSubclassOf('Illuminate\Database\Eloquent\Model'), sprintf('Expected %s to extend the given class', $object->name));
        }
    }
    public function test_controllers_are_final(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Http\Controllers');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isFinal(), sprintf('Expected %s to be final', $object->name));
        }
    }
    public function test_no_debugging(): void
    {
        $layer = $this->layer();
        $forbiddenFunctions = ['dd', 'dump', 'ray'];
        foreach ($layer as $object) {
            foreach ($object->uses as $use) {
                foreach ($forbiddenFunctions as $function) {
                    $this->assertNotSame($function, $use, 'Function ' . $function . ' should not be used in ' . $object->name);
                }
            }
        }
    }
}
