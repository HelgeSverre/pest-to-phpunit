<?php
arch('not not final')
    ->expect('App\Models')
    ->not->not->toBeFinal();
-----
<?php
class ArchDoubleNegationTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_not_not_final(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isFinal(), sprintf('Expected %s to be final', $object->name));
        }
    }
}
