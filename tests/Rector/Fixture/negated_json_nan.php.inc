<?php
test('negated json and special', function () {
    expect($x)->not->toBeJson();
    expect($x)->not->toBeNan();
});
-----
<?php
class NegatedJsonNanTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_json_and_special(): void
    {
        $this->assertIsNotString($x);
        $this->assertNan($x);
    }
}
