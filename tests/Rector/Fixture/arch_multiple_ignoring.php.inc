<?php
arch('strict with exclusions')
    ->expect('App')
    ->ignoring('App\Legacy')
    ->ignoring('App\Generated')
    ->toUseStrictTypes();
-----
<?php
class ArchMultipleIgnoringTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_strict_with_exclusions(): void
    {
        $layer = $this->layer()->leaveByNameStart('App')->excludeByNameStart('App\Legacy')->excludeByNameStart('App\Generated');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertTrue(str_contains($contents, 'declare(strict_types=1)'), 'Expected strict_types in ' . $object->name);
        }
    }
}
