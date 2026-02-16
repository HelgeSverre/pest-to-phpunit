<?php
test('throws only class', function () {
    throw new InvalidArgumentException();
})->throws(InvalidArgumentException::class);
-----
<?php
class ThrowsOnlyClassTest extends \PHPUnit\Framework\TestCase
{
    public function test_throws_only_class(): void
    {
        $this->expectException(InvalidArgumentException::class);
        throw new InvalidArgumentException();
    }
}
