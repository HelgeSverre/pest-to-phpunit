<?php
uses(Tests\TestCase::class);

test('dont see text', function () {
    $response = $this->get('/');
    expect($response)->assertDontSee('Error');
});

test('see in order', function () {
    $response = $this->get('/');
    expect($response)->assertSeeInOrder(['Header', 'Body', 'Footer']);
});

test('see text', function () {
    $response = $this->get('/');
    expect($response)->assertSeeText('Welcome');
});

test('inline expect on request', function () {
    expect($this->get('/'))->assertOk()->assertSee('Welcome');
});

test('long multi-assert chain', function () {
    $response = $this->get('/');
    expect($response)->assertOk()->assertSee('Welcome')->assertDontSee('Error')->assertSeeInOrder(['A', 'B']);
});

-----
<?php

class LaravelContentEdgeCasesTest extends \Tests\TestCase
{
    public function test_dont_see_text(): void
    {
        $response = $this->get('/');
        $response->assertDontSee('Error');
    }
    public function test_see_in_order(): void
    {
        $response = $this->get('/');
        $response->assertSeeInOrder(['Header', 'Body', 'Footer']);
    }
    public function test_see_text(): void
    {
        $response = $this->get('/');
        $response->assertSeeText('Welcome');
    }
    public function test_inline_expect_on_request(): void
    {
        $this->get('/')->assertOk()->assertSee('Welcome');
    }
    public function test_long_multiassert_chain(): void
    {
        $response = $this->get('/');
        $response->assertOk()->assertSee('Welcome')->assertDontSee('Error')->assertSeeInOrder(['A', 'B']);
    }
}
