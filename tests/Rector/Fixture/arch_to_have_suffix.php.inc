<?php
arch('controllers have suffix')
    ->expect('App\Http\Controllers')
    ->toHaveSuffix('Controller');
-----
<?php
class ArchToHaveSuffixTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_controllers_have_suffix(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Http\Controllers');
        foreach ($layer as $object) {
            $this->assertTrue(str_ends_with(class_basename($object->name), 'Controller'), sprintf('Expected %s to have the given suffix', $object->name));
        }
    }
}
