<?php
arch('models are not final')
    ->expect('App\Models')
    ->not->toBeFinal();
-----
<?php
class ArchNegatedTargetedTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_are_not_final(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertFalse($object->reflectionClass->isFinal(), sprintf('Expected %s not to be final', $object->name));
        }
    }
}
