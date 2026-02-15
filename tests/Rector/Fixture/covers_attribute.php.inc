<?php

covers(App\Models\User::class);

test('user has name', function () {
    expect(true)->toBeTrue();
});

-----
<?php

#[\PHPUnit\Framework\Attributes\CoversClass(App\Models\User::class)]
class CoversAttributeTest extends \PHPUnit\Framework\TestCase
{
    public function test_user_has_name(): void
    {
        $this->assertTrue(true);
    }
}
