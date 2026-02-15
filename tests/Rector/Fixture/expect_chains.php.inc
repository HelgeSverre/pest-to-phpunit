<?php

test('various assertions', function () {
    expect($value)->toBeNull();
    expect($flag)->toBeTrue();
    expect($flag)->toBeFalse();
    expect($list)->toBeEmpty();
    expect($name)->toBeString();
    expect($count)->toBeInt();
    expect($item)->toBeInstanceOf(Foo::class);
    expect($text)->toContain('hello');
    expect($list)->toHaveCount(3);
    expect($text)->toStartWith('Hello');
    expect($text)->toEndWith('world');
    expect($text)->toMatch('/hello/');
    expect($num)->toBeGreaterThan(5);
    expect($num)->toBeLessThan(10);
    expect($arr)->toHaveKey('name');
});

-----
<?php

class ExpectChainsTest extends \PHPUnit\Framework\TestCase
{
    public function test_various_assertions(): void
    {
        $this->assertNull($value);
        $this->assertTrue($flag);
        $this->assertFalse($flag);
        $this->assertEmpty($list);
        $this->assertIsString($name);
        $this->assertIsInt($count);
        $this->assertInstanceOf(Foo::class, $item);
        $this->assertContains('hello', $text);
        $this->assertCount(3, $list);
        $this->assertStringStartsWith('Hello', $text);
        $this->assertStringEndsWith('world', $text);
        $this->assertMatchesRegularExpression('/hello/', $text);
        $this->assertGreaterThan(5, $num);
        $this->assertLessThan(10, $num);
        $this->assertArrayHasKey('name', $arr);
    }
}
