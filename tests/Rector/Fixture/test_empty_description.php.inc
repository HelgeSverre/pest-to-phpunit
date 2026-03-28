<?php
test('', function () {
    expect(true)->toBeTrue();
});

-----
<?php
class TestEmptyDescriptionTest extends \PHPUnit\Framework\TestCase
{
    public function test_(): void
    {
        $this->assertTrue(true);
    }
}
