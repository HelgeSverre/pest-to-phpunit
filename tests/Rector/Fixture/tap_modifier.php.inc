<?php

test('tap runs side effects', function () {
    $log = [];
    expect($value)->tap(function () use (&$log) {
        $log[] = 'checked';
    })->toBe(42);
});

-----
<?php

class TapModifierTest extends \PHPUnit\Framework\TestCase
{
    public function test_tap_runs_side_effects(): void
    {
        $log = [];
        $log[] = 'checked';
        $this->assertSame(42, $value);
    }
}

