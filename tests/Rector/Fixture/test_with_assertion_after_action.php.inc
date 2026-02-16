<?php
uses(Tests\TestCase::class);

test('creates a post', function () {
    $response = $this->post('/posts', [
        'title' => 'My Post',
        'body' => 'Content here',
    ]);

    expect($response->status())->toBe(201);
    expect($response->json('title'))->toBe('My Post');
});
-----
<?php

class TestWithAssertionAfterActionTest extends \Tests\TestCase
{
    public function test_creates_a_post(): void
    {
        $response = $this->post('/posts', [
            'title' => 'My Post',
            'body' => 'Content here',
        ]);
        $this->assertSame(201, $response->status());
        $this->assertSame('My Post', $response->json('title'));
    }
}
