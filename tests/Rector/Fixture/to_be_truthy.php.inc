<?php
test('truthy values', function () {
    expect(1)->toBeTruthy();
    expect('hello')->toBeTruthy();
});

-----
<?php
class ToBeTruthyTest extends \PHPUnit\Framework\TestCase
{
    public function test_truthy_values(): void
    {
        $this->assertNotEmpty(1);
        $this->assertNotEmpty('hello');
    }
}
