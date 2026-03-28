<?php
arch('strict types')
    ->expect('App')
    ->toUseStrictTypes();

arch('no debugging')
    ->expect(['dd', 'dump'])
    ->not->toBeUsed();
-----
<?php

class ArchOnlyArchTestsTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_strict_types(): void
    {
        $layer = $this->layer()->leaveByNameStart('App');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertTrue(str_contains($contents, 'declare(strict_types=1)'), 'Expected strict_types in ' . $object->name);
        }
    }
    public function test_no_debugging(): void
    {
        $layer = $this->layer();
        $forbiddenFunctions = ['dd', 'dump'];
        foreach ($layer as $object) {
            foreach ($object->uses as $use) {
                foreach ($forbiddenFunctions as $function) {
                    $this->assertNotSame($function, $use, 'Function ' . $function . ' should not be used in ' . $object->name);
                }
            }
        }
    }
}
