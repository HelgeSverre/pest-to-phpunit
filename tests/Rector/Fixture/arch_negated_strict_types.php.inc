<?php

arch('no strict types in legacy')
    ->expect('App\Legacy')
    ->not->toUseStrictTypes();

-----
<?php

class ArchNegatedStrictTypesTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_no_strict_types_in_legacy(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Legacy');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertFalse(str_contains($contents, 'declare(strict_types=1)'), 'Expected no strict_types in ' . $object->name);
        }
    }
}
