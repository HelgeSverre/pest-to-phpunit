<?php
test('mixed assertions', function () {
    $result = process();
    $this->assertNotNull($result);
    expect($result)->toBeArray();
    $this->assertGreaterThan(0, count($result));
    expect($result)->toHaveKey('status');
});
-----
<?php
class MixedExpectAndPhpunitAssertionsTest extends \PHPUnit\Framework\TestCase
{
    public function test_mixed_assertions(): void
    {
        $result = process();
        $this->assertNotNull($result);
        $this->assertIsArray($result);
        $this->assertGreaterThan(0, count($result));
        $this->assertArrayHasKey('status', $result);
    }
}
