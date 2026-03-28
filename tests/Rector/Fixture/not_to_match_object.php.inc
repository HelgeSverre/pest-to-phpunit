<?php
test('does not match object', function () {
    expect($result)->not->toMatchObject(['key' => 'value']);
});

-----
<?php
class NotToMatchObjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_does_not_match_object(): void
    {
        $this->assertNotEquals(['key' => 'value'], $result);
    }
}
