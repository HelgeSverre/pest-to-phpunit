<?php

test('models are final')
    ->expect('App\Models')
    ->toBeFinal();

-----
<?php

class ArchHigherOrderStringExpectTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_are_final(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isFinal(), sprintf('Expected %s to be final', $object->name));
        }
    }
}
