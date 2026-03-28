<?php

uses(\Tests\Traits\RefreshDatabase::class);

arch('models are final')
    ->expect('App\Models')
    ->toBeFinal();

test('normal test', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class ArchWithUsesTraitTest extends \PHPUnit\Framework\TestCase
{
    use Tests\Traits\RefreshDatabase, \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_are_final(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isFinal(), sprintf('Expected %s to be final', $object->name));
        }
    }
    public function test_normal_test(): void
    {
        $this->assertTrue(true);
    }
}
