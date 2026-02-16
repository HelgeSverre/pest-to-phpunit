<?php
test('skip and group', function () {
    expect(true)->toBeTrue();
})->skip('WIP')->group('integration');
-----
<?php
class ChainedSkipAndGroupTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\Group('integration')]
    public function test_skip_and_group(): void
    {
        $this->markTestSkipped('WIP');
    }
}
