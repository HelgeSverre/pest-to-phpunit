<?php
test('complex modifiers', function () {
    expect(true)->toBeTrue();
})->group('unit', 'fast')->skip('not ready');

-----
<?php
class MultipleGroupsTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\Group('unit')]
    #[\PHPUnit\Framework\Attributes\Group('fast')]
    public function test_complex_modifiers(): void
    {
        $this->markTestSkipped('not ready');
    }
}
