<?php

arch('value objects use nothing')
    ->expect('App\ValueObjects')
    ->toUseNothing();

-----
<?php

class ArchToUseNothingTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_value_objects_use_nothing(): void
    {
        $this->markTestIncomplete('Arch assertion \'useNothing\' requires manual review');
    }
}
