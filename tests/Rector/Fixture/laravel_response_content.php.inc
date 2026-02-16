<?php
uses(Tests\TestCase::class);

test('response contains text', function () {
    $response = $this->get('/');
    expect($response)->assertSee('Welcome');
});

test('response has json', function () {
    $response = $this->getJson('/api/users');
    expect($response)->assertJsonCount(3, 'data');
});

test('response has view', function () {
    $response = $this->get('/');
    expect($response)->assertViewIs('welcome');
});

test('chained assertions', function () {
    $response = $this->get('/');
    expect($response)->assertOk()->assertSee('Welcome');
});

-----
<?php

class LaravelResponseContentTest extends \Tests\TestCase
{
    public function test_response_contains_text(): void
    {
        $response = $this->get('/');
        $response->assertSee('Welcome');
    }
    public function test_response_has_json(): void
    {
        $response = $this->getJson('/api/users');
        $response->assertJsonCount(3, 'data');
    }
    public function test_response_has_view(): void
    {
        $response = $this->get('/');
        $response->assertViewIs('welcome');
    }
    public function test_chained_assertions(): void
    {
        $response = $this->get('/');
        $response->assertOk()->assertSee('Welcome');
    }
}
