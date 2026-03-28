<?php
test('has keys', function () {
    expect($data)->toHaveKeys(['name', 'email', 'age']);
});

-----
<?php
class ToHaveKeysPositiveTest extends \PHPUnit\Framework\TestCase
{
    public function test_has_keys(): void
    {
        $this->assertArrayHasKey('name', $data);
        $this->assertArrayHasKey('email', $data);
        $this->assertArrayHasKey('age', $data);
    }
}
