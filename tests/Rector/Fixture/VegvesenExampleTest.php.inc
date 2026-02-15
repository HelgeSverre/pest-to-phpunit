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
    public function test_it_can_test(): void
    {
        $this->assertTrue(true);
    }
    public function test_all_resource_classes_extend_the_base_resource(): void
    {
    }
    public function test_all_request_classes_extend_the_saloon_request_class(): void
    {
    }
}
test('does not use dd or dump')
    ->expect(['dd', 'dump'])
    ->not->toBeUsed();
