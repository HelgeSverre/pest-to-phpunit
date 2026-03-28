<?php

describe('Architecture', function () {
    test('models are classes')
        ->expect('App\Models')
        ->toBeClasses();
});

-----
<?php

class ArchHigherOrderInDescribeTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_architecture_models_are_classes(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue(class_exists($object->name) && !enum_exists($object->name), sprintf('Expected %s to be a class', $object->name));
        }
    }
}
