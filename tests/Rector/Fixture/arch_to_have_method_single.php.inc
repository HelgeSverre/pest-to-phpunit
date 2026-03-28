<?php
arch('has execute method')
    ->expect('App\Commands')
    ->toHaveMethod('execute');
-----
<?php
class ArchToHaveMethodSingleTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_has_execute_method(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Commands');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->hasMethod('execute'), sprintf('Expected %s to have the given method', $object->name));
        }
    }
}
