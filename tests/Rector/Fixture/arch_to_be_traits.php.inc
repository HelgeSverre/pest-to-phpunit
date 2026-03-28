<?php
arch('concerns are traits')
    ->expect('App\Concerns')
    ->toBeTraits();
-----
<?php
class ArchToBeTraitsTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_concerns_are_traits(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Concerns');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isTrait(), sprintf('Expected %s to be a trait', $object->name));
        }
    }
}
