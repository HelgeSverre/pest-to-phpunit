<?php
arch('app models')
    ->expect('App\Models')
    ->toBeClasses();

arch('strict types')
    ->expect('App')
    ->toUseStrictTypes();
-----
<?php

class ArchWithChainsTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_app_models(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue(class_exists($object->name) && !enum_exists($object->name), sprintf('Expected %s to be a class', $object->name));
        }
    }
    public function test_strict_types(): void
    {
        $layer = $this->layer()->leaveByNameStart('App');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertTrue(str_contains($contents, 'declare(strict_types=1)'), 'Expected strict_types in ' . $object->name);
        }
    }
}
