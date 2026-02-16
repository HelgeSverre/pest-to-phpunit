<?php
test('key with value', function () {
    expect(['name' => 'John', 'age' => 30])->toHaveKey('name');
    expect(['name' => 'John'])->toHaveKey('name');
});
-----
<?php
class ToHaveKeyWithValueTest extends \PHPUnit\Framework\TestCase
{
    public function test_key_with_value(): void
    {
        $this->assertArrayHasKey('name', ['name' => 'John', 'age' => 30]);
        $this->assertArrayHasKey('name', ['name' => 'John']);
    }
}
