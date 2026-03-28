<?php

it('no debug functions')
    ->expect(['dd', 'dump'])
    ->each->not->toBeUsed();

test('classes are final')
    ->expect('App\Http')
    ->toBeFinal();

-----
<?php

class ArchHigherOrderMultipleTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_it_no_debug_functions(): void
    {
        $layer = $this->layer();
        $forbiddenFunctions = ['dd', 'dump'];
        foreach ($layer as $object) {
            foreach ($object->uses as $use) {
                foreach ($forbiddenFunctions as $function) {
                    $this->assertNotSame($function, $use, 'Function ' . $function . ' should not be used in ' . $object->name);
                }
            }
        }
    }
    public function test_classes_are_final(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Http');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isFinal(), sprintf('Expected %s to be final', $object->name));
        }
    }
}
