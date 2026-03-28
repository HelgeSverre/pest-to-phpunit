<?php

arch('statuses')
    ->expect('App\Enums')
    ->enums()
    ->toBeEnums();

-----
<?php

class ArchEnumsFilterTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_statuses(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Enums')->leaveByType(\PHPUnit\Architecture\Enums\ObjectType::_ENUM);
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isEnum(), sprintf('Expected %s to be an enum', $object->name));
        }
    }
}
