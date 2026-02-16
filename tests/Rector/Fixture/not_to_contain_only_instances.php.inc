<?php

test('not only instances', function () {
    expect($items)->not->toContainOnlyInstancesOf(User::class);
});

-----
<?php

class NotToContainOnlyInstancesTest extends \PHPUnit\Framework\TestCase
{
    public function test_not_only_instances(): void
    {
        $this->assertContainsNotOnlyInstancesOf(User::class, $items);
    }
}
