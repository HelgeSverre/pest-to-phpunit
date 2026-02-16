<?php
uses(Tests\TestCase::class);

test('response is ok', function () {
    $response = $this->get('/');
    expect($response)->assertOk();
});

test('response is created', function () {
    $response = $this->post('/items', ['name' => 'new']);
    expect($response)->assertCreated();
});

test('response is not found', function () {
    $response = $this->get('/missing');
    expect($response)->assertNotFound();
});

test('response has status', function () {
    $response = $this->get('/');
    expect($response)->assertStatus(200);
});

-----
<?php

class LaravelResponseStatusTest extends \Tests\TestCase
{
    public function test_response_is_ok(): void
    {
        $response = $this->get('/');
        $response->assertOk();
    }
    public function test_response_is_created(): void
    {
        $response = $this->post('/items', ['name' => 'new']);
        $response->assertCreated();
    }
    public function test_response_is_not_found(): void
    {
        $response = $this->get('/missing');
        $response->assertNotFound();
    }
    public function test_response_has_status(): void
    {
        $response = $this->get('/');
        $response->assertStatus(200);
    }
}
