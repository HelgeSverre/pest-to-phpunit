<?php

arch('classes use strict types')
    ->expect('App')
    ->classes()
    ->ignoring('App\Legacy')
    ->ignoring('App\Generated')
    ->toUseStrictTypes();

-----
<?php

class ArchFullChainTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_classes_use_strict_types(): void
    {
        $layer = $this->layer()->leaveByNameStart('App')->leaveByType(\PHPUnit\Architecture\Enums\ObjectType::_CLASS)->excludeByNameStart('App\Legacy')->excludeByNameStart('App\Generated');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertTrue(str_contains($contents, 'declare(strict_types=1)'), 'Expected strict_types in ' . $object->name);
        }
    }
}
