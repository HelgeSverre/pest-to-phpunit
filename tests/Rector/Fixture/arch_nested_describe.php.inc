<?php

describe('Architecture', function () {
    describe('Models', function () {
        arch('models extend base')
            ->expect('App\Models')
            ->toExtend('Illuminate\Database\Eloquent\Model');
    });
});

-----
<?php

class ArchNestedDescribeTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_models_extend_base(): void
    {
        $layer = $this->layer()->leaveByNameStart('App\Models');
        foreach ($layer as $object) {
            $this->assertTrue($object->reflectionClass->isSubclassOf('Illuminate\Database\Eloquent\Model'), sprintf('Expected %s to extend the given class', $object->name));
        }
    }
}
