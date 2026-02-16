<?php
describe('arrow hooks', function () {
    beforeEach(fn () => $this->value = 42);
    afterEach(fn () => $this->value = null);

    test('uses value', function () {
        expect($this->value)->toBe(42);
    });
});

-----
<?php
class DescribeArrowFunctionHooksTest extends \PHPUnit\Framework\TestCase
{
    protected $value;
    public function test_arrow_hooks_uses_value(): void
    {
        try {
            $this->value = 42;
            $this->assertSame(42, $this->value);
        } finally {
            $this->value = null;
        }
    }
}

