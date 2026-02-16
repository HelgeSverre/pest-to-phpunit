<?php
uses(Tests\TestCase::class, Illuminate\Foundation\Testing\RefreshDatabase::class);
-----
<?php
class OnlyUsesNoTestsTest extends \Tests\TestCase
{
    use Illuminate\Foundation\Testing\RefreshDatabase;
}
