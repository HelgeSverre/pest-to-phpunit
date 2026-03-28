<?php

it('will not use debugging functions')
    ->expect(['dd', 'dump', 'ray'])
    ->each->not->toBeUsed();

-----
<?php

class HigherOrderArchTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_it_will_not_use_debugging_functions(): void
    {
        $layer = $this->layer();
        $forbiddenFunctions = ['dd', 'dump', 'ray'];
        foreach ($layer as $object) {
            foreach ($object->uses as $use) {
                foreach ($forbiddenFunctions as $function) {
                    $this->assertNotSame($function, $use, 'Function ' . $function . ' should not be used in ' . $object->name);
                }
            }
        }
    }
}
