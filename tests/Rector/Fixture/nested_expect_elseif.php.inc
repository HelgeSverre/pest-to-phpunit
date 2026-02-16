<?php

test('elseif expect', function () {
    $val = rand(1, 100);
    if ($val > 75) {
        expect($val)->toBeGreaterThan(75);
    } elseif ($val > 25) {
        expect($val)->toBeGreaterThan(25);
    } else {
        expect($val)->toBeLessThanOrEqual(25);
    }
});

-----
<?php

class NestedExpectElseifTest extends \PHPUnit\Framework\TestCase
{
    public function test_elseif_expect(): void
    {
        $val = rand(1, 100);
        if ($val > 75) {
            $this->assertGreaterThan(75, $val);
        } elseif ($val > 25) {
            $this->assertGreaterThan(25, $val);
        } else {
            $this->assertLessThanOrEqual(25, $val);
        }
    }
}
