<?php

test('negated assertions', function () {
    expect($value)->not->toBeNull();
    expect($flag)->not->toBeTrue();
    expect($list)->not->toBeEmpty();
    expect($name)->not->toBe('admin');
    expect($item)->not->toBeInstanceOf(Foo::class);
});

-----
<?php

class NotModifierTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_assertions(): void
    {
        $this->assertNotNull($value);
        $this->assertNotTrue($flag);
        $this->assertNotEmpty($list);
        $this->assertNotSame('admin', $name);
        $this->assertNotInstanceOf(Foo::class, $item);
    }
}
