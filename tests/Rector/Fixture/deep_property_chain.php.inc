<?php
test('deep property access', function () {
    expect($response)->data->user->name->toBe('John');
});

-----
<?php
class DeepPropertyChainTest extends \PHPUnit\Framework\TestCase
{
    public function test_deep_property_access(): void
    {
        $this->assertSame('John', $response->data->user->name);
    }
}
