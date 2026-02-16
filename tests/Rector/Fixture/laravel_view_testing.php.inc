<?php
uses(Tests\TestCase::class);

test('has view', function () {
    $response = $this->get('/profile');
    expect($response)->assertViewIs('profile.show');
});

test('view has data', function () {
    $response = $this->get('/profile');
    expect($response)->assertViewHas('user');
});

test('view has multiple', function () {
    $response = $this->get('/profile');
    expect($response)->assertViewHasAll(['user', 'posts']);
});

test('view missing data', function () {
    $response = $this->get('/profile');
    expect($response)->assertViewMissing('admin');
});

test('view has data with closure', function () {
    $response = $this->get('/profile');
    expect($response)->assertViewHas('user', fn ($user) => $user->id === 1);
});

-----
<?php

class LaravelViewTestingTest extends \Tests\TestCase
{
    public function test_has_view(): void
    {
        $response = $this->get('/profile');
        $response->assertViewIs('profile.show');
    }
    public function test_view_has_data(): void
    {
        $response = $this->get('/profile');
        $response->assertViewHas('user');
    }
    public function test_view_has_multiple(): void
    {
        $response = $this->get('/profile');
        $response->assertViewHasAll(['user', 'posts']);
    }
    public function test_view_missing_data(): void
    {
        $response = $this->get('/profile');
        $response->assertViewMissing('admin');
    }
    public function test_view_has_data_with_closure(): void
    {
        $response = $this->get('/profile');
        $response->assertViewHas('user', fn ($user) => $user->id === 1);
    }
}
