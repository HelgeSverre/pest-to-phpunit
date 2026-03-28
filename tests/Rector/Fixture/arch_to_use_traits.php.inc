<?php
arch('models use soft deletes')
    ->expect('App\Models')
    ->toUseTrait('Illuminate\Database\Eloquent\SoftDeletes');
-----
<?php
class ArchToUseTraitsTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_use_soft_deletes(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue(in_array('Illuminate\Database\Eloquent\SoftDeletes', $object->reflectionClass->getTraitNames(), true), sprintf('Expected %s to use the given trait', $object->name));
        }
    }
}
