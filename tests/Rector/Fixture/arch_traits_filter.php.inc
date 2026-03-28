<?php

arch('concerns')
    ->expect('App\Concerns')
    ->traits()
    ->toBeTraits();

-----
<?php

class ArchTraitsFilterTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_concerns(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Concerns')->leaveByType(\PHPUnit\Architecture\Enums\ObjectType::_TRAIT);
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isTrait(), sprintf('Expected %s to be a trait', $object->name));
        }
    }
}
