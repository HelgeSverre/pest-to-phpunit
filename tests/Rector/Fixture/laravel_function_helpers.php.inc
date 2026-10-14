<?php
use function Pest\Laravel\get;
use function Pest\Laravel\postJson as sendJson;
use function Pest\Laravel\{actingAs, assertDatabaseHas};
uses(\Tests\TestCase::class);
test('helpers', function () {
    get('/')->assertOk();
    sendJson('/api', ['name' => 'Alice'])->assertCreated();
    actingAs($user)->get('/profile')->assertOk();
    assertDatabaseHas('users', ['id' => 1]);
    \Pest\Laravel\get('/health')->assertOk();
    expect(get('/'))->toBeObject();
});
-----
<?php

class LaravelFunctionHelpersTest extends \Tests\TestCase
{
    public function test_helpers(): void
    {
        $this->get('/')->assertOk();
        $this->postJson('/api', ['name' => 'Alice'])->assertCreated();
        $this->actingAs($user)->get('/profile')->assertOk();
        $this->assertDatabaseHas('users', ['id' => 1]);
        $this->get('/health')->assertOk();
        $this->assertIsObject($this->get('/'));
    }
}
