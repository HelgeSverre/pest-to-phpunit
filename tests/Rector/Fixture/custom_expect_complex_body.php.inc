<?php
// Source: spatie/pest-expectations - toPassWith
expect()->extend('toPassWith', function (mixed $value) {
    $rule = $this->value;
    $passed = true;
    $fail = function () use (&$passed) {
        $passed = false;
    };
    $rule->validate('attribute', $value, $fail);
    expect($passed)->toBeTrue();
    return $this;
});

test('rule passes', function () {
    expect($rule)->toPassWith('valid@example.com');
});

-----
<?php

class CustomExpectComplexBodyTest extends \PHPUnit\Framework\TestCase
{
    public function test_rule_passes(): void
    {
        // TODO(Pest): Custom expectation ->toPassWith() was defined via expect()->extend() and requires manual conversion
        $__pest_subject = $rule;
        $rule = $__pest_subject;
        $passed = true;
        $fail = function () use (&$passed) {
            $passed = false;
        };
        $rule->validate('attribute', 'valid@example.com', $fail);
        $this->assertTrue($passed);
    }
}
