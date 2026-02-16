<?php
test('compact usage', function () {
    $name = 'John';
    $age = 30;
    $data = compact('name', 'age');

    expect($data)->toHaveKey('name');
    expect($data)->toHaveKey('age');
    expect($data['name'])->toBe('John');
});
-----
<?php
class TestWithCompactTest extends \PHPUnit\Framework\TestCase
{
    public function test_compact_usage(): void
    {
        $name = 'John';
        $age = 30;
        $data = compact('name', 'age');
        $this->assertArrayHasKey('name', $data);
        $this->assertArrayHasKey('age', $data);
        $this->assertSame('John', $data['name']);
    }
}
