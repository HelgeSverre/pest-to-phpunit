<?php
test('negated empty null', function () {
    expect($x)->not->toBeEmpty();
    expect($x)->not->toBeNull();
    expect($x)->not->toBeTrue();
    expect($x)->not->toBeFalse();
});
-----
<?php
class NegatedEmptyNullTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_empty_null(): void
    {
        $this->assertNotEmpty($x);
        $this->assertNotNull($x);
        $this->assertNotTrue($x);
        $this->assertNotFalse($x);
    }
}
