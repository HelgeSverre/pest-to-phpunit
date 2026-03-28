<?php

arch('helpers used nowhere')
    ->expect('App\Helpers')
    ->toBeUsedInNothing();

-----
<?php

class ArchToBeUsedInNothingTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_helpers_used_nowhere(): void
    {
        $this->markTestIncomplete('Arch assertion \'beUsedInNothing\' requires manual review');
    }
}
