<?php
test('pipe transforms subject', function () {
    expect('hello world')->pipe('strtoupper')->toBe('HELLO WORLD');
});

-----
<?php
class PipeThenAssertTest extends \PHPUnit\Framework\TestCase
{
    public function test_pipe_transforms_subject(): void
    {
        $this->assertSame('HELLO WORLD', ('strtoupper')('hello world'));
    }
}
