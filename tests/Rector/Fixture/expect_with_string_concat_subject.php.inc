<?php
test('string concat subject', function () {
    expect('hello' . ' ' . 'world')->toBe('hello world');
});
-----
<?php
class ExpectWithStringConcatSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_string_concat_subject(): void
    {
        $this->assertSame('hello world', 'hello' . ' ' . 'world');
    }
}
