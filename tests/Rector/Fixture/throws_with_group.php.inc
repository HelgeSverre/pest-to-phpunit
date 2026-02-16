<?php
test('error handling', function () {
    throw new RuntimeException('fail');
})->throws(RuntimeException::class, 'fail')->group('errors');
-----
<?php
class ThrowsWithGroupTest extends \PHPUnit\Framework\TestCase
{
    #[\PHPUnit\Framework\Attributes\Group('errors')]
    public function test_error_handling(): void
    {
        $this->expectException(RuntimeException::class);
        $this->expectExceptionMessage('fail');
        throw new RuntimeException('fail');
    }
}
