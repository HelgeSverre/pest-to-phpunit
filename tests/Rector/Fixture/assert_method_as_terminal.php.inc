<?php

test('assert on expect subject', function () {
    expect($response)->assertOk();
});

test('assert with args', function () {
    expect($response)->assertSee('hello', false);
});

test('multiple assert calls chained', function () {
    expect($response)->assertOk()->assertSee('hello');
});

test('assert after property access', function () {
    expect($response)->headers->assertOk();
});

test('mixed expect and assert', function () {
    expect($value)->toBe(42);
    expect($response)->assertOk();
});

-----
<?php

class AssertMethodAsTerminalTest extends \PHPUnit\Framework\TestCase
{
    public function test_assert_on_expect_subject(): void
    {
        $response->assertOk();
    }
    public function test_assert_with_args(): void
    {
        $response->assertSee('hello', false);
    }
    public function test_multiple_assert_calls_chained(): void
    {
        $response->assertOk()->assertSee('hello');
    }
    public function test_assert_after_property_access(): void
    {
        $response->headers->assertOk();
    }
    public function test_mixed_expect_and_assert(): void
    {
        $this->assertSame(42, $value);
        $response->assertOk();
    }
}
