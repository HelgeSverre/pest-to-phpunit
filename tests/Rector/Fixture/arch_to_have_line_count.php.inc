<?php
arch('files are small')
    ->expect('App')
    ->toHaveLineCountLessThan(500);
-----
<?php
class ArchToHaveLineCountTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_files_are_small(): void
    {
        $layer = $this->layer()->leaveByNameStart('App');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertLessThan(500, substr_count($contents, '
') + 1, $object->name . ' exceeds line count limit');
        }
    }
}
