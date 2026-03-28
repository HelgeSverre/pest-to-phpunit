<?php

arch('commands have methods')
    ->expect('App\Commands')
    ->toHaveMethod(['handle', 'execute']);

-----
<?php

class ArchToHaveMethodArrayTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_commands_have_methods(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Commands');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->hasMethod('handle') && $object->reflectionClass->hasMethod('execute'), sprintf('Expected %s to have the given method', $object->name));
        }
    }
}
