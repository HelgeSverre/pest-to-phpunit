<?php
test('it adds guard to agent', function (): void {
    $agent = createAgent();
    $agent->guard('pii');

    expect($agent->getGuards())->toHaveCount(1)
        ->and($agent->getGuards()[0])->toBeInstanceOf(PIIGuard::class);
});

test('it throws when guard is violated', function (): void {
    $agent = createAgent();
    $agent->guard('pii');

    expect(fn () => $agent->prompt('email?'))
        ->toThrow(GuardException::class);
});

test('it throws with message', function (): void {
    expect(fn () => doSomething())
        ->toThrow(RuntimeException::class, 'Guard class');
});

test('it supports multiple guards', function (): void {
    $agent = createAgent();
    $agent->guard('pii')->guard('filter');

    expect($agent->getGuards())->toHaveCount(2);
});

test('it stops at first violation', function (): void {
    $firstCalled = false;
    $secondCalled = false;

    expect(fn () => runGuards())
        ->toThrow(GuardException::class);

    expect($firstCalled)->toBeTrue();
    expect($secondCalled)->toBeFalse();
});

-----
<?php

class AgentGuardsTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_adds_guard_to_agent(): void
    {
        $agent = createAgent();
        $agent->guard('pii');
        $this->assertCount(1, $agent->getGuards());
        $this->assertInstanceOf(PIIGuard::class, $agent->getGuards()[0]);
    }
    public function test_it_throws_when_guard_is_violated(): void
    {
        $agent = createAgent();
        $agent->guard('pii');
        $this->expectException(GuardException::class);
        (fn () => $agent->prompt('email?'))();
    }
    public function test_it_throws_with_message(): void
    {
        $this->expectException(RuntimeException::class);
        $this->expectExceptionMessage('Guard class');
        (fn () => doSomething())();
    }
    public function test_it_supports_multiple_guards(): void
    {
        $agent = createAgent();
        $agent->guard('pii')->guard('filter');
        $this->assertCount(2, $agent->getGuards());
    }
    public function test_it_stops_at_first_violation(): void
    {
        $firstCalled = false;
        $secondCalled = false;
        $this->expectException(GuardException::class);
        (fn () => runGuards())();
        $this->assertTrue($firstCalled);
        $this->assertFalse($secondCalled);
    }
}
