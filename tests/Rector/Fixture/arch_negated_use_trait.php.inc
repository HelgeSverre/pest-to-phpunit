<?php

arch('no soft deletes')
    ->expect('App\ValueObjects')
    ->not->toUseTrait('Illuminate\Database\Eloquent\SoftDeletes');

-----
<?php

class ArchNegatedUseTraitTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_no_soft_deletes(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\ValueObjects');
        foreach ($layer as $object) {
            $this->assertFalse(in_array('Illuminate\Database\Eloquent\SoftDeletes', $object->reflectionClass->getTraitNames(), true), sprintf('Expected %s not to use the given trait', $object->name));
        }
    }
}
