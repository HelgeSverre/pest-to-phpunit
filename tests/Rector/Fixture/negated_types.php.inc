<?php
test('negated types', function () {
    expect($x)->not->toBeString();
    expect($x)->not->toBeInt();
    expect($x)->not->toBeFloat();
    expect($x)->not->toBeArray();
    expect($x)->not->toBeBool();
    expect($x)->not->toBeObject();
    expect($x)->not->toBeCallable();
    expect($x)->not->toBeIterable();
    expect($x)->not->toBeNumeric();
    expect($x)->not->toBeResource();
    expect($x)->not->toBeScalar();
});
-----
<?php
class NegatedTypesTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_types(): void
    {
        $this->assertIsNotString($x);
        $this->assertIsNotInt($x);
        $this->assertIsNotFloat($x);
        $this->assertIsNotArray($x);
        $this->assertIsNotBool($x);
        $this->assertIsNotObject($x);
        $this->assertIsNotCallable($x);
        $this->assertIsNotIterable($x);
        $this->assertIsNotNumeric($x);
        $this->assertIsNotResource($x);
        $this->assertIsNotScalar($x);
    }
}
