<?php

const DEFAULT_TIMEOUT = 30;

test('uses constant', function () {
    expect(DEFAULT_TIMEOUT)->toBe(30);
});
-----
<?php

const DEFAULT_TIMEOUT = 30;

class TestWithConstantTest extends \PHPUnit\Framework\TestCase
{
    public function test_uses_constant(): void
    {
        $this->assertSame(30, DEFAULT_TIMEOUT);
    }
}
