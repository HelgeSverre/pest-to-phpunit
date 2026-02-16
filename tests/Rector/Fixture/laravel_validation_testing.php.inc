<?php
uses(Tests\TestCase::class);

test('has validation errors', function () {
    $response = $this->post('/register', []);
    expect($response)->assertSessionHasErrors(['email', 'password']);
});

test('invalid fields', function () {
    $response = $this->post('/register', []);
    expect($response)->assertInvalid(['email', 'password']);
});

test('valid fields', function () {
    $response = $this->post('/register', ['email' => 'test@test.com', 'password' => 'secret123']);
    expect($response)->assertValid(['email']);
});

test('json validation errors', function () {
    $response = $this->postJson('/api/register', []);
    expect($response)->assertUnprocessable()->assertJsonValidationErrors(['email']);
});

-----
<?php

class LaravelValidationTestingTest extends \Tests\TestCase
{
    public function test_has_validation_errors(): void
    {
        $response = $this->post('/register', []);
        $response->assertSessionHasErrors(['email', 'password']);
    }
    public function test_invalid_fields(): void
    {
        $response = $this->post('/register', []);
        $response->assertInvalid(['email', 'password']);
    }
    public function test_valid_fields(): void
    {
        $response = $this->post('/register', ['email' => 'test@test.com', 'password' => 'secret123']);
        $response->assertValid(['email']);
    }
    public function test_json_validation_errors(): void
    {
        $response = $this->postJson('/api/register', []);
        $response->assertUnprocessable()->assertJsonValidationErrors(['email']);
    }
}
