<?php
covers(App\Models\User::class);
covers(App\Models\Team::class);

test('user and team', function () {
    expect(true)->toBeTrue();
});
-----
<?php

#[\PHPUnit\Framework\Attributes\CoversClass(App\Models\User::class)]
#[\PHPUnit\Framework\Attributes\CoversClass(App\Models\Team::class)]
class MultipleCoversTest extends \PHPUnit\Framework\TestCase
{
    public function test_user_and_team(): void
    {
        $this->assertTrue(true);
    }
}
