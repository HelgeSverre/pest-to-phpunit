<?php
test('uuid check', function () {
    expect('550e8400-e29b-41d4-a716-446655440000')->toBeUuid();
});
-----
<?php
class ToBeUuidTest extends \PHPUnit\Framework\TestCase
{
    public function test_uuid_check(): void
    {
        $this->assertMatchesRegularExpression('/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/i', '550e8400-e29b-41d4-a716-446655440000');
    }
}
