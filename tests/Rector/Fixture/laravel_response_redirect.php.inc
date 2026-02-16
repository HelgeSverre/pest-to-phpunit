<?php
uses(Tests\TestCase::class);

test('response redirects', function () {
    $response = $this->get('/old');
    expect($response)->assertRedirect('/new');
});

test('response has header', function () {
    $response = $this->get('/');
    expect($response)->assertHeader('Content-Type', 'text/html');
});

test('response session', function () {
    $response = $this->post('/login', ['email' => 'test@test.com']);
    expect($response)->assertSessionHas('status');
});

-----
<?php

class LaravelResponseRedirectTest extends \Tests\TestCase
{
    public function test_response_redirects(): void
    {
        $response = $this->get('/old');
        $response->assertRedirect('/new');
    }
    public function test_response_has_header(): void
    {
        $response = $this->get('/');
        $response->assertHeader('Content-Type', 'text/html');
    }
    public function test_response_session(): void
    {
        $response = $this->post('/login', ['email' => 'test@test.com']);
        $response->assertSessionHas('status');
    }
}
