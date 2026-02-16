<?php
test('not json', function () {
    expect($value)->not->toBeJson();
});
-----
<?php
class NotToBeJsonTest extends \PHPUnit\Framework\TestCase
{
    public function test_not_json(): void
    {
        // TODO(Pest): not->toBeJson() has no direct PHPUnit equivalent
    }
}