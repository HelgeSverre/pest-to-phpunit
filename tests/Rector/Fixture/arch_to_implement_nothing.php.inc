<?php
arch('models implement nothing')
    ->expect('App\Models')
    ->toImplementNothing();
-----
<?php
class ArchToImplementNothingTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_implement_nothing(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->getInterfaceNames() === [], sprintf('Expected %s to implement nothing', $object->name));
        }
    }
}
