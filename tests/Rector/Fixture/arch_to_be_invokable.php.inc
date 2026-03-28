<?php
arch('actions are invokable')
    ->expect('App\Actions')
    ->toBeInvokable();
-----
<?php
class ArchToBeInvokableTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_actions_are_invokable(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Actions');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->hasMethod('__invoke'), sprintf('Expected %s to be invokable', $object->name));
        }
    }
}
