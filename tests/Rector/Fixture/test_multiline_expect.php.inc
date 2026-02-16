<?php
test('multiline chain', function () {
    expect($response)
        ->toBeObject()
        ->and($response->status)
        ->toBe(200)
        ->and($response->body)
        ->toBeString();
});
-----
<?php
class TestMultilineExpectTest extends \PHPUnit\Framework\TestCase
{
    public function test_multiline_chain(): void
    {
        $this->assertIsObject($response);
        $this->assertSame(200, $response->status);
        $this->assertIsString($response->body);
    }
}
