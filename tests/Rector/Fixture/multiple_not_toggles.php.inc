<?php
test('double negation', function () {
    expect(true)->not->not->toBeTrue();
});

-----
<?php
class MultipleNotTogglesTest extends \PHPUnit\Framework\TestCase
{
    public function test_double_negation(): void
    {
        $this->assertTrue(true);
    }
}
