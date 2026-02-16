<?php

use App\Models\Post;

uses(Tests\TestCase::class, Illuminate\Foundation\Testing\RefreshDatabase::class);

covers(Post::class);

beforeEach(function () {
    $this->post = Post::factory()->create();
});

afterEach(function () {
    Post::query()->delete();
});

test('can create post', function () {
    $post = Post::create(['title' => 'New Post', 'body' => 'Content']);
    expect($post)->toBeInstanceOf(Post::class);
    expect($post->title)->toBe('New Post');
});

test('can update post', function () {
    $this->post->update(['title' => 'Updated']);
    expect($this->post->fresh()->title)->toBe('Updated');
});

test('can delete post', function () {
    $this->post->delete();
    expect(Post::find($this->post->id))->toBeNull();
});

test('validates title required', function () {
    expect(fn () => Post::create(['body' => 'no title']))
        ->toThrow(\Illuminate\Validation\ValidationException::class);
});
-----
<?php

use App\Models\Post;
#[\PHPUnit\Framework\Attributes\CoversClass(Post::class)]
class RealWorldCrudTest extends \Tests\TestCase
{
    use Illuminate\Foundation\Testing\RefreshDatabase;
    protected $post;
    protected function setUp(): void
    {
        parent::setUp();
        $this->post = Post::factory()->create();
    }
    protected function tearDown(): void
    {
        parent::tearDown();
        Post::query()->delete();
    }
    public function test_can_create_post(): void
    {
        $post = Post::create(['title' => 'New Post', 'body' => 'Content']);
        $this->assertInstanceOf(Post::class, $post);
        $this->assertSame('New Post', $post->title);
    }
    public function test_can_update_post(): void
    {
        $this->post->update(['title' => 'Updated']);
        $this->assertSame('Updated', $this->post->fresh()->title);
    }
    public function test_can_delete_post(): void
    {
        $this->post->delete();
        $this->assertNull(Post::find($this->post->id));
    }
    public function test_validates_title_required(): void
    {
        $this->expectException(\Illuminate\Validation\ValidationException::class);
        (fn () => Post::create(['body' => 'no title']))();
    }
}
