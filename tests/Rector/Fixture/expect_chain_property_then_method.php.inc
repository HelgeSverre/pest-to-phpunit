<?php
test('property then method accessor', function () {
    expect($response)->status->toBe(200);
    expect($user)->name->toBeString();
    expect($collection)->items->toHaveCount(3);
});

-----
<?php
class ExpectChainPropertyThenMethodTest extends \PHPUnit\Framework\TestCase
{
    public function test_property_then_method_accessor(): void
    {
        $this->assertSame(200, $response->status);
        $this->assertIsString($user->name);
        $this->assertCount(3, $collection->items);
    }
}
