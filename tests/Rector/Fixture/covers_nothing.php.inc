<?php
coversNothing();

test('utility test', function () {
    expect(1 + 1)->toBe(2);
});

-----
<?php

#[\PHPUnit\Framework\Attributes\CoversNothing]
class CoversNothingTest extends \PHPUnit\Framework\TestCase
{
    public function test_utility_test(): void
    {
        $this->assertSame(2, 1 + 1);
    }
}
