<?php

uses(Tests\TestCase::class, Illuminate\Foundation\Testing\RefreshDatabase::class);

beforeEach(function () {
    $this->user = \App\Models\User::factory()->create();
    $this->actingAs($this->user);
});

test('index returns paginated list', function () {
    $response = $this->getJson('/api/items');

    expect($response->status())->toBe(200);
    expect($response->json())->toHaveKey('data');
    expect($response->json())->toHaveKey('meta');
    expect($response->json('meta.current_page'))->toBe(1);
});

test('store creates item', function () {
    $response = $this->postJson('/api/items', [
        'name' => 'New Item',
        'price' => 9.99,
    ]);

    expect($response->status())->toBe(201);
    expect($response->json('data.name'))->toBe('New Item');
});

test('store validation fails', function () {
    $response = $this->postJson('/api/items', []);

    expect($response->status())->toBe(422);
    expect($response->json('errors'))->toHaveKey('name');
});

test('show returns item', function () {
    $item = \App\Models\Item::factory()->create();

    $response = $this->getJson("/api/items/{$item->id}");

    expect($response->status())->toBe(200);
    expect($response->json('data.id'))->toBe($item->id);
});

test('destroy deletes item', function () {
    $item = \App\Models\Item::factory()->create();

    $response = $this->deleteJson("/api/items/{$item->id}");

    expect($response->status())->toBe(204);
});
-----
<?php

class RealWorldHttpTest extends \Tests\TestCase
{
    use Illuminate\Foundation\Testing\RefreshDatabase;
    protected $user;
    protected function setUp(): void
    {
        parent::setUp();
        $this->user = \App\Models\User::factory()->create();
        $this->actingAs($this->user);
    }
    public function test_index_returns_paginated_list(): void
    {
        $response = $this->getJson('/api/items');
        $this->assertSame(200, $response->status());
        $this->assertArrayHasKey('data', $response->json());
        $this->assertArrayHasKey('meta', $response->json());
        $this->assertSame(1, $response->json('meta.current_page'));
    }
    public function test_store_creates_item(): void
    {
        $response = $this->postJson('/api/items', [
            'name' => 'New Item',
            'price' => 9.99,
        ]);
        $this->assertSame(201, $response->status());
        $this->assertSame('New Item', $response->json('data.name'));
    }
    public function test_store_validation_fails(): void
    {
        $response = $this->postJson('/api/items', []);
        $this->assertSame(422, $response->status());
        $this->assertArrayHasKey('name', $response->json('errors'));
    }
    public function test_show_returns_item(): void
    {
        $item = \App\Models\Item::factory()->create();
        $response = $this->getJson("/api/items/{$item->id}");
        $this->assertSame(200, $response->status());
        $this->assertSame($item->id, $response->json('data.id'));
    }
    public function test_destroy_deletes_item(): void
    {
        $item = \App\Models\Item::factory()->create();
        $response = $this->deleteJson("/api/items/{$item->id}");
        $this->assertSame(204, $response->status());
    }
}
