<?php
class CompatibilityLaravelTestCase extends \Illuminate\Foundation\Testing\TestCase
{
    public function createApplication()
    {
        return new \Illuminate\Foundation\Application();
    }
}
uses(CompatibilityLaravelTestCase::class);
test('response assertions', function () {
    $response = new \Illuminate\Testing\TestResponse(new \Illuminate\Http\Response('hello', 200));
    $response->assertOk()->assertSee('hello');
    expect($response->getContent())->toBe('hello');
});
-----
<?php
class CompatibilityLaravelTestCase extends \Illuminate\Foundation\Testing\TestCase
{
    public function createApplication()
    {
        return new \Illuminate\Foundation\Application();
    }
}
class LaravelResponseRuntimeTest extends \CompatibilityLaravelTestCase
{
    public function test_response_assertions(): void
    {
        $response = new \Illuminate\Testing\TestResponse(new \Illuminate\Http\Response('hello', 200));
        $response->assertOk()->assertSee('hello');
        $this->assertSame('hello', $response->getContent());
    }
}
