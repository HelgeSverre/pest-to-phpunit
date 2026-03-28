<?php

it('can test', function () {
    expect(true)->toBeTrue();
});
test('does not use dd or dump')
    ->expect(['dd', 'dump'])
    ->not->toBeUsed();

test('All resource classes extend the base resource')
    ->expect('HelgeSverre\Vegvesen\Resource')
    ->toExtend('HelgeSverre\Vegvesen\Resource');

test('All request classes extend the saloon request class')
    ->expect('HelgeSverre\Vegvesen\Requests')
    ->classes()
    ->toExtend('Saloon\Http\Request');

-----
<?php

class VegvesenExampleTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_it_can_test(): void
    {
        $this->assertTrue(true);
    }
    public function test_does_not_use_dd_or_dump(): void
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
    public function test_all_resource_classes_extend_the_base_resource(): void
    {
        $layer = $this->layer()->leaveByNameStart('HelgeSverre\Vegvesen\Resource');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isSubclassOf('HelgeSverre\Vegvesen\Resource'), sprintf('Expected %s to extend the given class', $object->name));
        }
    }
    public function test_all_request_classes_extend_the_saloon_request_class(): void
    {
        $layer = $this->layer()->leaveByNameStart('HelgeSverre\Vegvesen\Requests');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isSubclassOf('Saloon\Http\Request'), sprintf('Expected %s to extend the given class', $object->name));
        }
    }
}
