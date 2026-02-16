<?php

test('when conditional', function () {
    expect($value)->when($debug, function ($v) {
        $v->toBe(42);
    })->toBeInt();
});

test('unless conditional', function () {
    expect($value)->unless($skip, function ($v) {
        $v->toBeString();
    })->toBeString();
});

test('pipe transforms', function () {
    expect($items)->pipe(fn ($v) => count($v))->toBe(3);
});

-----
<?php

class WhenUnlessPipeTest extends \PHPUnit\Framework\TestCase
{
    public function test_when_conditional(): void
    {
        // TODO(Pest): ->when() requires manual conversion to PHPUnit
        $this->assertIsInt($value);
    }
    public function test_unless_conditional(): void
    {
        // TODO(Pest): ->unless() requires manual conversion to PHPUnit
        $this->assertIsString($value);
    }
    public function test_pipe_transforms(): void
    {
        $this->assertSame(3, (fn ($v) => count($v))($items));
    }
}