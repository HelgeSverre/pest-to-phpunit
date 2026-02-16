<?php
uses(Tests\TestCase::class);

test('json structure', function () {
    $response = $this->getJson('/api/posts');
    expect($response)->assertOk()->assertJsonStructure(['data' => ['*' => ['id', 'title']]]);
});

test('json path', function () {
    $response = $this->getJson('/api/posts/1');
    expect($response)->assertJsonPath('data.id', 1);
});

test('json fragment', function () {
    $response = $this->getJson('/api/posts');
    expect($response)->assertJsonFragment(['title' => 'Hello World']);
});

test('exact json', function () {
    $response = $this->getJson('/api/status');
    expect($response)->assertExactJson(['status' => 'ok']);
});

test('json missing', function () {
    $response = $this->getJson('/api/posts');
    expect($response)->assertJsonMissing(['debug' => true]);
});

test('json count', function () {
    $response = $this->getJson('/api/posts');
    expect($response)->assertOk()->assertJsonCount(5, 'data');
});

-----
<?php

class LaravelJsonTestingTest extends \Tests\TestCase
{
    public function test_json_structure(): void
    {
        $response = $this->getJson('/api/posts');
        $response->assertOk()->assertJsonStructure(['data' => ['*' => ['id', 'title']]]);
    }
    public function test_json_path(): void
    {
        $response = $this->getJson('/api/posts/1');
        $response->assertJsonPath('data.id', 1);
    }
    public function test_json_fragment(): void
    {
        $response = $this->getJson('/api/posts');
        $response->assertJsonFragment(['title' => 'Hello World']);
    }
    public function test_exact_json(): void
    {
        $response = $this->getJson('/api/status');
        $response->assertExactJson(['status' => 'ok']);
    }
    public function test_json_missing(): void
    {
        $response = $this->getJson('/api/posts');
        $response->assertJsonMissing(['debug' => true]);
    }
    public function test_json_count(): void
    {
        $response = $this->getJson('/api/posts');
        $response->assertOk()->assertJsonCount(5, 'data');
    }
}
