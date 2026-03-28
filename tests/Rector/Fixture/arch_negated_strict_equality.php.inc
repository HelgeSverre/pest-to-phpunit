<?php

arch('loose equality ok')
    ->expect('App\Legacy')
    ->not->toUseStrictEquality();

-----
<?php

class ArchNegatedStrictEqualityTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_loose_equality_ok(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Legacy');
        foreach ($layer as $object) {
            $contents = file_get_contents($object->path);
            $this->assertNotSame(0, preg_match('/[^=!]==[^=]|[^!]!=[^=]/', $contents), 'Loose equality found in ' . $object->name);
        }
    }
}
