<?php
test('multi group', function () {
    expect(true)->toBeTrue();
})->group('unit', 'fast', 'smoke');
-----
<?php
class MultipleGroupsOnTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\Group('unit')]
    #[\PHPUnit\Framework\Attributes\Group('fast')]
    #[\PHPUnit\Framework\Attributes\Group('smoke')]
    public function test_multi_group(): void
    {
        $this->assertTrue(true);
    }
}
