<?php

arch('int backed enums')
    ->expect('App\Enums')
    ->toBeIntBackedEnum();

-----
<?php

class ArchBackedEnumIntTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_int_backed_enums(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Enums');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isEnum(), sprintf('Expected %s to be an int-backed enum (checked isEnum only)', $object->name));
        }
    }
}
