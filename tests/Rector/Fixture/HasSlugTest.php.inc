<?php

use Larapress\Enums\PostType;
use Larapress\Models\Post;
use Larapress\Models\Taxonomy;
use Larapress\Models\Term;

beforeEach(function () {
    $this->user = $this->createUser();
});

test('auto-generates slug from title when slug is empty', function () {
    $post = Post::create([
        'author_id' => $this->user->id,
        'title' => 'My Amazing Post',
        'type' => PostType::Post,
    ]);

    expect($post->slug)->toBe('my-amazing-post');
});

test('does not overwrite existing slug', function () {
    $post = Post::create([
        'author_id' => $this->user->id,
        'title' => 'My Amazing Post',
        'slug' => 'custom-slug',
        'type' => PostType::Post,
    ]);

    expect($post->slug)->toBe('custom-slug');
});

test('generates unique slug by appending counter on collision', function () {
    Post::create([
        'author_id' => $this->user->id,
        'title' => 'Duplicate Title',
        'slug' => 'duplicate-title',
        'type' => PostType::Post,
    ]);

    $post2 = Post::create([
        'author_id' => $this->user->id,
        'title' => 'Duplicate Title',
        'type' => PostType::Post,
    ]);

    expect($post2->slug)->toBe('duplicate-title-1');
});

test('same slug allowed for different post types', function () {
    Post::create([
        'author_id' => $this->user->id,
        'title' => 'About',
        'slug' => 'about',
        'type' => PostType::Post,
    ]);

    $page = Post::create([
        'author_id' => $this->user->id,
        'title' => 'About',
        'type' => PostType::Page,
    ]);

    expect($page->slug)->toBe('about');
});

test('term slug uniqueness is scoped by taxonomy_id', function () {
    $categoryTaxonomy = Taxonomy::create([
        'name' => 'Category',
        'slug' => 'category',
        'hierarchical' => true,
    ]);

    $tagTaxonomy = Taxonomy::create([
        'name' => 'Tag',
        'slug' => 'tag',
        'hierarchical' => false,
    ]);

    $term1 = Term::create([
        'taxonomy_id' => $categoryTaxonomy->id,
        'name' => 'Technology',
    ]);

    $term2 = Term::create([
        'taxonomy_id' => $tagTaxonomy->id,
        'name' => 'Technology',
    ]);

    expect($term1->slug)->toBe('technology')
        ->and($term2->slug)->toBe('technology');
});

-----
<?php

use Larapress\Enums\PostType;
use Larapress\Models\Post;
use Larapress\Models\Taxonomy;
use Larapress\Models\Term;
class HasSlugTest extends \PHPUnit\Framework\TestCase
{
    protected function setUp(): void
    {
        parent::setUp();
        $this->user = $this->createUser();
    }
    public function test_autogenerates_slug_from_title_when_slug_is_empty(): void
    {
        $post = Post::create([
            'author_id' => $this->user->id,
            'title' => 'My Amazing Post',
            'type' => PostType::Post,
        ]);
        $this->assertSame('my-amazing-post', $post->slug);
    }
    public function test_does_not_overwrite_existing_slug(): void
    {
        $post = Post::create([
            'author_id' => $this->user->id,
            'title' => 'My Amazing Post',
            'slug' => 'custom-slug',
            'type' => PostType::Post,
        ]);
        $this->assertSame('custom-slug', $post->slug);
    }
    public function test_generates_unique_slug_by_appending_counter_on_collision(): void
    {
        Post::create([
            'author_id' => $this->user->id,
            'title' => 'Duplicate Title',
            'slug' => 'duplicate-title',
            'type' => PostType::Post,
        ]);
        $post2 = Post::create([
            'author_id' => $this->user->id,
            'title' => 'Duplicate Title',
            'type' => PostType::Post,
        ]);
        $this->assertSame('duplicate-title-1', $post2->slug);
    }
    public function test_same_slug_allowed_for_different_post_types(): void
    {
        Post::create([
            'author_id' => $this->user->id,
            'title' => 'About',
            'slug' => 'about',
            'type' => PostType::Post,
        ]);
        $page = Post::create([
            'author_id' => $this->user->id,
            'title' => 'About',
            'type' => PostType::Page,
        ]);
        $this->assertSame('about', $page->slug);
    }
    public function test_term_slug_uniqueness_is_scoped_by_taxonomyid(): void
    {
        $categoryTaxonomy = Taxonomy::create([
            'name' => 'Category',
            'slug' => 'category',
            'hierarchical' => true,
        ]);
        $tagTaxonomy = Taxonomy::create([
            'name' => 'Tag',
            'slug' => 'tag',
            'hierarchical' => false,
        ]);
        $term1 = Term::create([
            'taxonomy_id' => $categoryTaxonomy->id,
            'name' => 'Technology',
        ]);
        $term2 = Term::create([
            'taxonomy_id' => $tagTaxonomy->id,
            'name' => 'Technology',
        ]);
        $this->assertSame('technology', $term1->slug);
        $this->assertSame('technology', $term2->slug);
    }
}
