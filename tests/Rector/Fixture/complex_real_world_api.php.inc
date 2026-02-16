<?php

use App\Models\Post;

uses(Tests\TestCase::class, Illuminate\Foundation\Testing\RefreshDatabase::class);

beforeEach(function () {
    $this->user = \App\Models\User::factory()->create();
});

describe('Post API', function () {
    test('lists posts', function () {
        Post::factory()->count(5)->create();

        $response = $this->getJson('/api/posts');

        expect($response->status())->toBe(200);
        expect($response->json('data'))->toHaveCount(5);
    });

    test('creates a post', function () {
        $response = $this->postJson('/api/posts', [
            'title' => 'Test Post',
            'body' => 'Test Body',
        ]);

        expect($response->status())->toBe(201);
        expect($response->json('data.title'))->toBe('Test Post');
    });

    test('validates required fields', function () {
        $response = $this->postJson('/api/posts', []);

        expect($response->status())->toBe(422);
    });
})->group('api');
-----
<?php

use App\Models\Post;
class ComplexRealWorldApiTest extends \Tests\TestCase
{
    use Illuminate\Foundation\Testing\RefreshDatabase;
    protected $user;
    protected function setUp(): void
    {
        parent::setUp();
        $this->user = \App\Models\User::factory()->create();
    }
    #[\PHPUnit\Framework\Attributes\Group('api')]
    public function test_post_api_lists_posts(): void
    {
        Post::factory()->count(5)->create();
        $response = $this->getJson('/api/posts');
        $this->assertSame(200, $response->status());
        $this->assertCount(5, $response->json('data'));
    }
    #[\PHPUnit\Framework\Attributes\Group('api')]
    public function test_post_api_creates_a_post(): void
    {
        $response = $this->postJson('/api/posts', [
            'title' => 'Test Post',
            'body' => 'Test Body',
        ]);
        $this->assertSame(201, $response->status());
        $this->assertSame('Test Post', $response->json('data.title'));
    }
    #[\PHPUnit\Framework\Attributes\Group('api')]
    public function test_post_api_validates_required_fields(): void
    {
        $response = $this->postJson('/api/posts', []);
        $this->assertSame(422, $response->status());
    }
}
