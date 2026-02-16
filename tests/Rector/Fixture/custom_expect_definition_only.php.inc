<?php
// This file only has custom expectation definitions (like tests/Expectations.php)
expect()->extend('toBePositive', function () {
    return $this->toBeGreaterThan(0);
});

expect()->extend('toBeEven', function () {
    expect($this->value % 2)->toBe(0);
});
