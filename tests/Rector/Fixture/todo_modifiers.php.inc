<?php
test('match modifier', function () {
    expect($value)->match([1 => fn ($v) => $v->toBe(1)]);
});

test('scoped modifier', function () {
    expect($value)->scoped(fn ($v) => $v->toBe(42));
});

test('sequence modifier', function () {
    expect($items)->sequence(fn ($v) => $v->toBe(1), fn ($v) => $v->toBe(2));
});

test('each with closure', function () {
    expect($items)->each(fn ($item) => $item->toBe(1));
});

-----
<?php

class TodoModifiersTest extends \PHPUnit\Framework\TestCase
{
    public function test_match_modifier(): void
    {
        // TODO(Pest): ->match() requires manual conversion to PHPUnit
    }
    public function test_scoped_modifier(): void
    {
        // TODO(Pest): ->scoped() requires manual conversion to PHPUnit
    }
    public function test_sequence_modifier(): void
    {
        // TODO(Pest): ->sequence() requires manual conversion to PHPUnit
    }
    public function test_each_with_closure(): void
    {
        // TODO(Pest): ->each(closure) requires manual conversion to PHPUnit
    }
}