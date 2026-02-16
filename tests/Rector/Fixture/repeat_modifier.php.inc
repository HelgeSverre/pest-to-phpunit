<?php

test('flaky test', function () {
    $result = random_int(1, 10);
    expect($result)->toBeGreaterThan(0);
})->repeat(3);

-----
<?php

class RepeatModifierTest extends \PHPUnit\Framework\TestCase
{
    public function test_flaky_test(): void
    {
        for ($__repeat_i = 0; $__repeat_i < 3; $__repeat_i++) {
            $result = random_int(1, 10);
            $this->assertGreaterThan(0, $result);
        }
    }
}
