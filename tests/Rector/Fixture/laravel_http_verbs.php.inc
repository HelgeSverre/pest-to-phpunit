<?php
uses(Tests\TestCase::class);

test('get request', function () {
    $response = $this->get('/');
    expect($response->status())->toBe(200);
});

test('post request', function () {
    $response = $this->post('/submit', ['data' => 'value']);
    expect($response->status())->toBe(302);
});

test('put request', function () {
    $response = $this->put('/items/1', ['name' => 'updated']);
    expect($response->status())->toBe(200);
});

test('delete request', function () {
    $response = $this->delete('/items/1');
    expect($response->status())->toBe(204);
});
-----
<?php

class LaravelHttpVerbsTest extends \Tests\TestCase
{
    public function test_get_request(): void
    {
        $response = $this->get('/');
        $this->assertSame(200, $response->status());
    }
    public function test_post_request(): void
    {
        $response = $this->post('/submit', ['data' => 'value']);
        $this->assertSame(302, $response->status());
    }
    public function test_put_request(): void
    {
        $response = $this->put('/items/1', ['name' => 'updated']);
        $this->assertSame(200, $response->status());
    }
    public function test_delete_request(): void
    {
        $response = $this->delete('/items/1');
        $this->assertSame(204, $response->status());
    }
}
