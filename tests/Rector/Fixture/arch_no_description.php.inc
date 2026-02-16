<?php
arch()->expect('App\Models')->toExtend('Illuminate\Database\Eloquent\Model');
-----
<?php
class ArchNoDescriptionTest extends \PHPUnit\Framework\TestCase
{
    public function test_arch_test(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: arch test');
    }
}
