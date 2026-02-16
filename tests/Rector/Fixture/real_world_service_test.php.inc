<?php

use App\Services\PaymentService;
use App\Models\Order;

uses(Tests\TestCase::class);

beforeEach(function () {
    $this->service = new PaymentService();
});

describe('charge', function () {
    test('processes valid payment', function () {
        $order = Order::factory()->create(['total' => 100]);
        $result = $this->service->charge($order);

        expect($result)->toBeArray();
        expect($result)->toHaveKey('transaction_id');
        expect($result['status'])->toBe('success');
        expect($result['amount'])->toBe(100);
    });

    test('rejects zero amount', function () {
        $order = Order::factory()->create(['total' => 0]);

        expect(fn () => $this->service->charge($order))
            ->toThrow(InvalidArgumentException::class, 'Amount must be positive');
    });

    test('handles currency conversion', function (string $currency, float $rate) {
        $order = Order::factory()->create(['total' => 100, 'currency' => $currency]);
        $result = $this->service->charge($order);

        expect($result['converted_amount'])->toEqualWithDelta(100 * $rate, 0.01);
    })->with([
        ['USD', 1.0],
        ['EUR', 0.85],
        ['GBP', 0.73],
    ]);
});

describe('refund', function () {
    test('processes full refund', function () {
        $result = $this->service->refund('txn_123');
        expect($result)->toBeTrue();
    });

    test('rejects invalid transaction', function () {
        expect(fn () => $this->service->refund('invalid'))
            ->toThrow(RuntimeException::class);
    });
})->group('payments');
-----
<?php

use App\Services\PaymentService;
use App\Models\Order;
class RealWorldServiceTest extends \Tests\TestCase
{
    protected $service;
    public static function test_charge_handles_currency_conversion_provider(): array
    {
        return [
            ['USD', 1.0],
            ['EUR', 0.85],
            ['GBP', 0.73],
        ];
    }
    protected function setUp(): void
    {
        parent::setUp();
        $this->service = new PaymentService();
    }
    public function test_charge_processes_valid_payment(): void
    {
        $order = Order::factory()->create(['total' => 100]);
        $result = $this->service->charge($order);
        $this->assertIsArray($result);
        $this->assertArrayHasKey('transaction_id', $result);
        $this->assertSame('success', $result['status']);
        $this->assertSame(100, $result['amount']);
    }
    public function test_charge_rejects_zero_amount(): void
    {
        $order = Order::factory()->create(['total' => 0]);
        $this->expectException(InvalidArgumentException::class);
        $this->expectExceptionMessage('Amount must be positive');
        (fn () => $this->service->charge($order))();
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('test_charge_handles_currency_conversion_provider')]
    public function test_charge_handles_currency_conversion(string $currency, float $rate): void
    {
        $order = Order::factory()->create(['total' => 100, 'currency' => $currency]);
        $result = $this->service->charge($order);
        $this->assertEqualsWithDelta(100 * $rate, $result['converted_amount'], 0.01);
    }
    #[\PHPUnit\Framework\Attributes\Group('payments')]
    public function test_refund_processes_full_refund(): void
    {
        $result = $this->service->refund('txn_123');
        $this->assertTrue($result);
    }
    #[\PHPUnit\Framework\Attributes\Group('payments')]
    public function test_refund_rejects_invalid_transaction(): void
    {
        $this->expectException(RuntimeException::class);
        (fn () => $this->service->refund('invalid'))();
    }
}
