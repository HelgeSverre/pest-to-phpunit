<?php

arch('no abstract prefix')
    ->expect('App\Models')
    ->not->toHavePrefix('Abstract');

-----
<?php

class ArchNegatedHavePrefixTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_no_abstract_prefix(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertFalse(str_starts_with(class_basename($object->name), 'Abstract'), sprintf('Expected %s not to have the given prefix', $object->name));
        }
    }
}
