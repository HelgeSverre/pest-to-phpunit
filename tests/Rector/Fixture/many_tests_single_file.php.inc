<?php
test('test 1', function () { expect(1)->toBe(1); });
test('test 2', function () { expect(2)->toBe(2); });
test('test 3', function () { expect(3)->toBe(3); });
test('test 4', function () { expect(4)->toBe(4); });
test('test 5', function () { expect(5)->toBe(5); });
test('test 6', function () { expect(6)->toBe(6); });
test('test 7', function () { expect(7)->toBe(7); });
test('test 8', function () { expect(8)->toBe(8); });
test('test 9', function () { expect(9)->toBe(9); });
test('test 10', function () { expect(10)->toBe(10); });
-----
<?php

class ManyTestsSingleFileTest extends \PHPUnit\Framework\TestCase
{
    public function test_test_1(): void
    {
        $this->assertSame(1, 1);
    }
    public function test_test_2(): void
    {
        $this->assertSame(2, 2);
    }
    public function test_test_3(): void
    {
        $this->assertSame(3, 3);
    }
    public function test_test_4(): void
    {
        $this->assertSame(4, 4);
    }
    public function test_test_5(): void
    {
        $this->assertSame(5, 5);
    }
    public function test_test_6(): void
    {
        $this->assertSame(6, 6);
    }
    public function test_test_7(): void
    {
        $this->assertSame(7, 7);
    }
    public function test_test_8(): void
    {
        $this->assertSame(8, 8);
    }
    public function test_test_9(): void
    {
        $this->assertSame(9, 9);
    }
    public function test_test_10(): void
    {
        $this->assertSame(10, 10);
    }
}
