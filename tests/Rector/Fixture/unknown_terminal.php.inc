<?php

test('custom expectation', function () {
    expect($response)->toBeSuccessful();
    expect($value)->toBe(42);
});

-----
<?php

class UnknownTerminalTest extends \PHPUnit\Framework\TestCase
{
    public function test_custom_expectation(): void
    {
        // TODO(Pest): Unknown expectation ->toBeSuccessful() has no PHPUnit equivalent
        $this->assertSame(42, $value);
    }
}
