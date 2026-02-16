<?php

test('not length', function () {
    expect([1, 2])->not->toHaveLength(3);
});

-----
<?php

class ToHaveLengthNegatedTest extends \PHPUnit\Framework\TestCase
{
    public function test_not_length(): void
    {
        $this->assertNotSame(3, is_string([1, 2]) ? strlen([1, 2]) : count([1, 2]));
    }
}
