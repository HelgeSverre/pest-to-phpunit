<?php

uses(\App\Traits\HasFactory::class);

arch('strict types')
    ->expect('App')
    ->toUseStrictTypes();

test('normal test', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class ArchUsesWithArchTest extends \PHPUnit\Framework\TestCase
{
    use App\Traits\HasFactory, \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_strict_types(): void
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