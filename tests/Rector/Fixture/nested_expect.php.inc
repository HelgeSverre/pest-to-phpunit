<?php

test('expect inside if', function () {
    $value = rand(1, 100);
    if ($value > 50) {
        expect($value)->toBeGreaterThan(50);
    }
});

test('expect inside foreach', function () {
    $items = [1, 2, 3];
    foreach ($items as $item) {
        expect($item)->toBeInt();
    }
});

test('expect inside else branch', function () {
    $value = rand(1, 100);
    if ($value > 50) {
        expect($value)->toBeGreaterThan(50);
    } else {
        expect($value)->toBeLessThanOrEqual(50);
    }
});

-----
<?php

class NestedExpectTest extends \PHPUnit\Framework\TestCase
{
    public function test_expect_inside_if(): void
    {
        $value = rand(1, 100);
        if ($value > 50) {
            $this->assertGreaterThan(50, $value);
        }
    }
    public function test_expect_inside_foreach(): void
    {
        $items = [1, 2, 3];
        foreach ($items as $item) {
            $this->assertIsInt($item);
        }
    }
    public function test_expect_inside_else_branch(): void
    {
        $value = rand(1, 100);
        if ($value > 50) {
            $this->assertGreaterThan(50, $value);
        } else {
            $this->assertLessThanOrEqual(50, $value);
        }
    }
}
