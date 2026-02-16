<?php
test('each with closure', function () {
    expect([1, 2, 3])->each(fn ($n) => $n->toBeLessThan(4));
});

-----
<?php
class EachWithClosureTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_with_closure(): void
    {
        // TODO(Pest): ->each(closure) requires manual conversion to PHPUnit
    }
}
