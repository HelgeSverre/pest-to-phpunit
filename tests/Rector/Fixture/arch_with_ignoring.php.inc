<?php
arch('strict types except legacy')
    ->expect('App')
    ->ignoring('App\Legacy')
    ->toUseStrictTypes();
-----
<?php
class ArchWithIgnoringTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_strict_types_except_legacy(): void
    {
        $layer = $this->layer()->leaveByNameStart('App')->excludeByNameStart('App\Legacy');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertTrue(str_contains($contents, 'declare(strict_types=1)'), 'Expected strict_types in ' . $object->name);
        }
    }
}
