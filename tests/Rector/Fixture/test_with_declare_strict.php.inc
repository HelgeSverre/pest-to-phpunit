<?php

declare(strict_types=1);

test('strict types', function () {
    expect(1 + 1)->toBe(2);
});
-----
<?php

declare(strict_types=1);

class TestWithDeclareStrictTest extends \PHPUnit\Framework\TestCase
{
    public function test_strict_types(): void
    {
        $this->assertSame(2, 1 + 1);
    }
}
