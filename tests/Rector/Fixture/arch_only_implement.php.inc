<?php

arch('only implements base')
    ->expect('App\Contracts')
    ->toOnlyImplement('App\Contracts\Base');

-----
<?php

class ArchOnlyImplementTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_only_implements_base(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Contracts');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->implementsInterface('App\Contracts\Base'), sprintf('Expected %s to only implement the given interface', $object->name));
        }
    }
}
