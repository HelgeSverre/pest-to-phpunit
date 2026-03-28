<?php

arch('models do not extend controller')
    ->expect('App\Models')
    ->not->toExtend('App\Http\Controllers\Controller');

-----
<?php

class ArchNegatedExtendTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_do_not_extend_controller(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertFalse($object->reflectionClass->isSubclassOf('App\Http\Controllers\Controller'), sprintf('Expected %s not to extend the given class', $object->name));
        }
    }
}
