<?php
test('complex body', function () {
    $users = User::factory()->count(3)->create();
    $filtered = $users->filter(fn ($u) => $u->active);

    expect($filtered)->toHaveCount(3);
    expect($filtered->first()->name)->toBeString();
});
-----
<?php
class TestWithComplexBodyTest extends \PHPUnit\Framework\TestCase
{
    public function test_complex_body(): void
    {
        $users = User::factory()->count(3)->create();
        $filtered = $users->filter(fn ($u) => $u->active);
        $this->assertCount(3, $filtered);
        $this->assertIsString($filtered->first()->name);
    }
}
