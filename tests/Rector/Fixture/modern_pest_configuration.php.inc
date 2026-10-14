<?php
pest()->extend(\Tests\TestCase::class)->use(\Illuminate\Foundation\Testing\RefreshDatabase::class)->group('feature');
test('works', function () {
    expect(true)->toBeTrue();
});
-----
<?php

#[\PHPUnit\Framework\Attributes\Group('feature')]
class ModernPestConfigurationTest extends \Tests\TestCase
{
    use \Illuminate\Foundation\Testing\RefreshDatabase;
    public function test_works(): void
    {
        $this->assertTrue(true);
    }
}
