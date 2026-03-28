<?php

arch('models do not implement interface')
    ->expect('App\Models')
    ->not->toImplement('App\Contracts\Renderable');

-----
<?php

class ArchNegatedImplementTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_do_not_implement_interface(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertFalse($object->reflectionClass->implementsInterface('App\Contracts\Renderable'), sprintf('Expected %s not to implement the given interface', $object->name));
        }
    }
}
