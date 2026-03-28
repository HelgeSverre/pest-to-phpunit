<?php

arch('string backed enums')
    ->expect('App\Enums')
    ->toBeStringBackedEnum();

-----
<?php

class ArchBackedEnumStringTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_string_backed_enums(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Enums');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isEnum(), sprintf('Expected %s to be a string-backed enum (checked isEnum only)', $object->name));
        }
    }
}
