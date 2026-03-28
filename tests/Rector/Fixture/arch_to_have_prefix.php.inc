<?php
arch('controllers have prefix')
    ->expect('App\Http\Controllers')
    ->toHavePrefix('App');
-----
<?php
class ArchToHavePrefixTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_controllers_have_prefix(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Http\Controllers');
        foreach ($layer as $object) {
            $this->assertTrue(str_starts_with(class_basename($object->name), 'App'), sprintf('Expected %s to have the given prefix', $object->name));
        }
    }
}
