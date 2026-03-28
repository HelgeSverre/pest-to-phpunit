<?php

arch('no delete method')
    ->expect('App\ReadModels')
    ->not->toHaveMethod('delete');

-----
<?php

class ArchNegatedHaveMethodTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_no_delete_method(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\ReadModels');
        foreach ($layer as $object) {
            $this->assertFalse($object->reflectionClass->hasMethod('delete'), sprintf('Expected %s not to have the given method', $object->name));
        }
    }
}
