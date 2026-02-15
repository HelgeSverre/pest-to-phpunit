<?php

test('and chain', function () {
    expect($name)->toBe('John')->and($age)->toBe(30);
});

-----
<?php

class AndChainTest extends \PHPUnit\Framework\TestCase
{
    public function test_and_chain(): void
    {
        $this->assertSame('John', $name);
        $this->assertSame(30, $age);
    }
}
