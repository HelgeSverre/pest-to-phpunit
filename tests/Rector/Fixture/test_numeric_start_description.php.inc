<?php
test('123 is a number', function () {
    expect(123)->toBeInt();
});

-----
<?php
class TestNumericStartDescriptionTest extends \PHPUnit\Framework\TestCase
{
    public function test_123_is_a_number(): void
    {
        $this->assertIsInt(123);
    }
}
