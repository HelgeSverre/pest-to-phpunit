<?php
describe('Error handling', function () {
    test('throws on invalid', function () {
        throw new InvalidArgumentException('bad input');
    })->throws(InvalidArgumentException::class, 'bad input');
});
-----
<?php
class DescribeWithThrowsTest extends \PHPUnit\Framework\TestCase
{
    public function test_error_handling_throws_on_invalid(): void
    {
        $this->expectException(InvalidArgumentException::class);
        $this->expectExceptionMessage('bad input');
        throw new InvalidArgumentException('bad input');
    }
}
