<?php

test('has length', function () {
    expect([1, 2, 3])->toHaveLength(3);
});

-----
<?php

class ToHaveLengthTest extends \PHPUnit\Framework\TestCase
{
    public function test_has_length(): void
    {
        $this->assertSame(3, is_string([1, 2, 3]) ? strlen([1, 2, 3]) : count([1, 2, 3]));
    }
}
