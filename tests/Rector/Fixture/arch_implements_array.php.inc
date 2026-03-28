<?php
arch('models implement multiple')
    ->expect('App\Models')
    ->toImplement('App\Contracts\Auditable');
-----
<?php
class ArchImplementsArrayTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_implement_multiple(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->implementsInterface('App\Contracts\Auditable'), sprintf('Expected %s to implement the given interface', $object->name));
        }
    }
}
