<?php
uses(Tests\TestCase::class);

test('uses this method', function () {
    $response = $this->get('/api/users');
    expect($response->status())->toBe(200);
});
-----
<?php

class TestThisMethodCallTest extends \Tests\TestCase
{
    public function test_uses_this_method(): void
    {
        $response = $this->get('/api/users');
        $this->assertSame(200, $response->status());
    }
}
