<?php

arch('models are classes')
    ->expect('App\Models')
    ->toBeClasses()
    ->group('arch');

-----
<?php

class ArchWithGroupTest extends \PHPUnit\Framework\TestCase
{
    public function test_models_are_classes(): void
    {
        /** Pest arch() test - manual review needed */
        $this->markTestSkipped('Arch test not supported in PHPUnit: models are classes');
    }
}
