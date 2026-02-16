<?php
arch('models extend base')
    ->expect('App\Models')
    ->toExtend('Illuminate\Database\Eloquent\Model');

arch('controllers are final')
    ->expect('App\Http\Controllers')
    ->toBeFinal();

arch('no debugging')
    ->expect(['dd', 'dump', 'ray'])
    ->not->toBeUsed();
-----
<?php
class MultipleArchTestsTest extends \PHPUnit\Framework\TestCase
{
    public function test_models_extend_base(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: models extend base');
    }
    public function test_controllers_are_final(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: controllers are final');
    }
}
arch('no debugging')
    ->expect(['dd', 'dump', 'ray'])
    ->not->toBeUsed();
