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
    public function test_does_not_use_dd_or_dump(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: does not use dd or dump');
    }
    public function test_all_resource_classes_extend_the_base_resource(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: All resource classes extend the base resource');
    }
    public function test_all_request_classes_extend_the_saloon_request_class(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: All request classes extend the saloon request class');
    }
}
