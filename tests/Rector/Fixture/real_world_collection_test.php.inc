<?php

test('collection operations', function () {
    $items = collect([1, 2, 3, 4, 5]);

    expect($items->count())->toBe(5);
    expect($items->sum())->toBe(15);
    expect($items->avg())->toEqualWithDelta(3.0, 0.001);
    expect($items->filter(fn ($i) => $i > 3)->values()->all())->toBe([4, 5]);
    expect($items->map(fn ($i) => $i * 2)->all())->toBe([2, 4, 6, 8, 10]);
    expect($items->first())->toBe(1);
    expect($items->last())->toBe(5);
    expect($items->contains(3))->toBeTrue();
    expect($items->contains(99))->toBeFalse();
});
-----
<?php

class RealWorldCollectionTest extends \PHPUnit\Framework\TestCase
{
    public function test_collection_operations(): void
    {
        $items = collect([1, 2, 3, 4, 5]);
        $this->assertSame(5, $items->count());
        $this->assertSame(15, $items->sum());
        $this->assertEqualsWithDelta(3.0, $items->avg(), 0.001);
        $this->assertSame([4, 5], $items->filter(fn ($i) => $i > 3)->values()->all());
        $this->assertSame([2, 4, 6, 8, 10], $items->map(fn ($i) => $i * 2)->all());
        $this->assertSame(1, $items->first());
        $this->assertSame(5, $items->last());
        $this->assertTrue($items->contains(3));
        $this->assertFalse($items->contains(99));
    }
}
