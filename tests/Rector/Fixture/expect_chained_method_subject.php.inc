<?php
test('method chain subject', function () {
    expect(collect([1, 2, 3])->count())->toBe(3);
});

-----
<?php
class ExpectChainedMethodSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_method_chain_subject(): void
    {
        $this->assertSame(3, collect([1, 2, 3])->count());
    }
}
