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
        $this->assertNotCount(3, [1, 2]);
    }
}
