<?php
describe('Payments', function () {
    test('processes refund', function () {
        expect(true)->toBeTrue();
    })->skip('Payment gateway not configured');

    test('charges card', function () {
        expect(true)->toBeTrue();
    });
});
-----
<?php
class DescribeWithSkipTest extends \PHPUnit\Framework\TestCase
{
    public function test_payments_processes_refund(): void
    {
        $this->markTestSkipped('Payment gateway not configured');
    }
    public function test_payments_charges_card(): void
    {
        $this->assertTrue(true);
    }
}
