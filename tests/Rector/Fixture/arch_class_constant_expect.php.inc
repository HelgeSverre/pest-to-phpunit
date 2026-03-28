<?php
arch('with class constant')
    ->expect(\App\Models\User::class)
    ->toBeFinal();
-----
<?php
class ArchClassConstantExpectTest extends \PHPUnit\Framework\TestCase
{
    public function test_with_class_constant(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: with class constant');
    }
}
