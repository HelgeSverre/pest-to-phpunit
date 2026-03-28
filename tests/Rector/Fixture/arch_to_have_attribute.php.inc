<?php
arch('commands have attribute')
    ->expect('App\Console\Commands')
    ->toHaveAttribute('Symfony\Component\Console\Attribute\AsCommand');
-----
<?php
class ArchToHaveAttributeTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_commands_have_attribute(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Console\Commands');
        foreach ($layer as $object) {
            $this->assertTrue(count($object->reflectionClass->getAttributes('Symfony\Component\Console\Attribute\AsCommand')) !== 0, sprintf('Expected %s to have the given attribute', $object->name));
        }
    }
}
