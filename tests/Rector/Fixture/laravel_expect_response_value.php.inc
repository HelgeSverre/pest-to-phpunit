<?php
uses(Tests\TestCase::class);

test('expect response array access', function () {
    $response = $this->getJson('/api/user');
    expect($response['name'])->toBe('John');
});

test('expect response status method', function () {
    $response = $this->get('/');
    expect($response->status())->toBe(200);
});

test('mixed response and value expects', function () {
    $response = $this->getJson('/api/user');
    $response->assertOk();
    expect($response['name'])->toBe('John');
    expect($response['age'])->toBeGreaterThan(18);
});

-----
<?php

class LaravelExpectResponseValueTest extends \Tests\TestCase
{
    public function test_expect_response_array_access(): void
    {
        $response = $this->getJson('/api/user');
        $this->assertSame('John', $response['name']);
    }
    public function test_expect_response_status_method(): void
    {
        $response = $this->get('/');
        $this->assertSame(200, $response->status());
    }
    public function test_mixed_response_and_value_expects(): void
    {
        $response = $this->getJson('/api/user');
        $response->assertOk();
        $this->assertSame('John', $response['name']);
        $this->assertGreaterThan(18, $response['age']);
    }
}
