<?php

arch('app code is strict')
    ->expect('App')
    ->toUseStrictTypes();

test('normal test', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class ArchTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_app_code_is_strict(): void
    {
        $layer = $this->layer()->leaveByNameStart('App');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertTrue(str_contains($contents, 'declare(strict_types=1)'), 'Expected strict_types in ' . $object->name);
        }
    }
    public function test_normal_test(): void
    {
        $this->assertTrue(true);
    }
}
