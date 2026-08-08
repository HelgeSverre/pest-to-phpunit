<?php
test('returns expectation', function () {
    return expect(1)->toBe(1);
});
-----
<?php
class ReturnExpectationTest extends \PHPUnit\Framework\TestCase
{
    public function test_returns_expectation(): void
    {
        $this->assertSame(1, 1);
        return;
    }
}
