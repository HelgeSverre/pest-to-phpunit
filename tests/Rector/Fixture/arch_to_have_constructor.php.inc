<?php
arch('services have constructor')
    ->expect('App\Services')
    ->toHaveConstructor();
-----
<?php
class ArchToHaveConstructorTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_services_have_constructor(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Services');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->hasMethod('__construct'), sprintf('Expected %s to have a constructor', $object->name));
        }
    }
}
