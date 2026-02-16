<?php

it('returns empty array on load', function (): void {
    $adapter = new NullAdapter;
    $messages = $adapter->load('test-session');
    expect($messages)->toBeArray()->toBeEmpty();
});

it('returns false on exists check', function (): void {
    $adapter = new NullAdapter;
    expect($adapter->exists('session-1'))->toBeFalse();
    expect($adapter->exists('session-2'))->toBeFalse();
});

-----
<?php

class NullAdapterTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_returns_empty_array_on_load(): void
    {
        $adapter = new NullAdapter;
        $messages = $adapter->load('test-session');
        $this->assertIsArray($messages);
        $this->assertEmpty($messages);
    }
    public function test_it_returns_false_on_exists_check(): void
    {
        $adapter = new NullAdapter;
        $this->assertFalse($adapter->exists('session-1'));
        $this->assertFalse($adapter->exists('session-2'));
    }
}
