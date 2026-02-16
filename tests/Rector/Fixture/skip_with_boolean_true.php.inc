<?php
test('conditionally skipped', function () {
    expect(true)->toBeTrue();
})->skip(true);

-----
<?php
class SkipWithBooleanTrueTest extends \PHPUnit\Framework\TestCase
{
    public function test_conditionally_skipped(): void
    {
        $this->markTestSkipped('Skipped');
    }
}
