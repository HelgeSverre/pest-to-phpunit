<?php
test('complex modifiers', function () {
    expect(true)->toBeTrue();
})->group('integration')->depends('other test')->skip('not ready');
-----
<?php
class SkipGroupDependsCombinedTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\Group('integration')]
    #[\PHPUnit\Framework\Attributes\Depends('test_other_test')]
    public function test_complex_modifiers(): void
    {
        $this->markTestSkipped('not ready');
    }
}
