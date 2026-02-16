<?php
test('falsy values', function () {
    expect(0)->toBeFalsy();
    expect('')->toBeFalsy();
});

-----
<?php
class ToBeFalsyTest extends \PHPUnit\Framework\TestCase
{
    public function test_falsy_values(): void
    {
        $this->assertEmpty(0);
        $this->assertEmpty('');
    }
}
