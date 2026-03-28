<?php

arch('final and strict')
    ->expect('App\Models')
    ->toBeFinal()
    ->toUseStrictTypes();

-----
<?php

class ArchChainedTerminalsTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_final_and_strict(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertTrue(str_contains($contents, 'declare(strict_types=1)'), 'Expected strict_types in ' . $object->name);
        }
    }
}
