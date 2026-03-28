<?php

test('classes are final')
    ->expect('App\Http')
    ->toBeFinal();

-----
<?php

class ArchHigherOrderTestStringTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_classes_are_final(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Http');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isFinal(), sprintf('Expected %s to be final', $object->name));
        }
    }
}
