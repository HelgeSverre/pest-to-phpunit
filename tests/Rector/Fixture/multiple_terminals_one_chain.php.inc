<?php

test('chained terminals', function () {
    expect($value)->toBe(42)->toBeInt();
});

-----
<?php

class MultipleTerminalsOneChainTest extends \PHPUnit\Framework\TestCase
{
    public function test_chained_terminals(): void
    {
        $this->assertSame(42, $value);
        $this->assertIsInt($value);
    }
}
