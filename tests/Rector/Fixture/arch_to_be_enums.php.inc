<?php
arch('statuses are enums')
    ->expect('App\Enums')
    ->toBeEnums();
-----
<?php
class ArchToBeEnumsTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_statuses_are_enums(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Enums');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isEnum(), sprintf('Expected %s to be an enum', $object->name));
        }
    }
}
