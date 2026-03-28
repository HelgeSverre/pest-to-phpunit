<?php
arch('strict equality')
    ->expect('App')
    ->toUseStrictEquality();
-----
<?php
class ArchToUseStrictEqualityTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_strict_equality(): void
    {
        $layer = $this->layer()->leaveByNameStart('App');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertSame(0, preg_match('/[^=!]==[^=]|[^!]!=[^=]/', $contents), 'Loose equality found in ' . $object->name);
        }
    }
}
