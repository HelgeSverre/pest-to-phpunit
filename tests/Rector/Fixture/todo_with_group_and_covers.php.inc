<?php
test('future feature', function () {
})->todo()->group('future')->covers(App\Models\User::class);
-----
<?php
class TodoWithGroupAndCoversTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\Group('future')]
    #[\PHPUnit\Framework\Attributes\CoversClass(App\Models\User::class)]
    public function test_future_feature(): void
    {
        $this->markTestIncomplete('TODO');
    }
}
