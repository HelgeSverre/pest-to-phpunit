<?php
test('ternary in expect', function () {
    expect($flag ? 'yes' : 'no')->toBe('yes');
});

-----
<?php
class ExpectTernarySubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_ternary_in_expect(): void
    {
        $this->assertSame('yes', $flag ? 'yes' : 'no');
    }
}
