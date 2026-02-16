<?php

test('equal canonicalizing', function () {
    expect([3, 1, 2])->toEqualCanonicalizing([1, 2, 3]);
});

-----
<?php

class ToEqualCanonicalizingTest extends \PHPUnit\Framework\TestCase
{
    public function test_equal_canonicalizing(): void
    {
        $this->assertEqualsCanonicalizing([1, 2, 3], [3, 1, 2]);
    }
}
