<?php
uses(Tests\TestCase::class);

test('follow redirects', function () {
    $response = $this->post('/login', ['email' => 'test@test.com']);
    expect($response)->assertRedirect()->followRedirects()->assertOk()->assertSee('Dashboard');
});

test('response with acting as', function () {
    $user = User::factory()->create();
    $response = $this->actingAs($user)->get('/dashboard');
    expect($response)->assertOk();
});

-----
<?php

class LaravelMixedChainTest extends \Tests\TestCase
{
    public function test_follow_redirects(): void
    {
        $response = $this->post('/login', ['email' => 'test@test.com']);
        $response->assertRedirect()->followRedirects()->assertOk()->assertSee('Dashboard');
    }
    public function test_response_with_acting_as(): void
    {
        $user = User::factory()->create();
        $response = $this->actingAs($user)->get('/dashboard');
        $response->assertOk();
    }
}
