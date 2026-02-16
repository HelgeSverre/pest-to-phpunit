<?php
test('static call in body', function () {
    $users = User::factory()->count(3)->create();
    expect($users)->toHaveCount(3);
});
-----
<?php
class TestWithStaticCallTest extends \PHPUnit\Framework\TestCase
{
    public function test_static_call_in_body(): void
    {
        $users = User::factory()->count(3)->create();
        $this->assertCount(3, $users);
    }
}
