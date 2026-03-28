<?php

arch('repos only used in services')
    ->expect('App\Repositories')
    ->toOnlyBeUsedIn('App\Services');

-----
<?php

class ArchToOnlyBeUsedInTest extends \PHPUnit\Framework\TestCase
{
    use \PHPUnit\Architecture\ArchitectureAsserts;
    public function test_repos_only_used_in_services(): void
    {
        $this->markTestIncomplete('Arch assertion \'onlyBeUsedIn\' requires manual review');
    }
}
