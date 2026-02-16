<?php
test('kebab case keys', function () {
    expect(['my-key' => 1])->toHaveKebabCaseKeys();
});

test('snake case keys', function () {
    expect(['my_key' => 1])->toHaveSnakeCaseKeys();
});

-----
<?php

class ToHaveCaseKeysTest extends \PHPUnit\Framework\TestCase
{
    public function test_kebab_case_keys(): void
    {
        foreach (array_keys(['my-key' => 1]) as $__key) {
            $this->assertMatchesRegularExpression('/^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$/', $__key);
        }
    }
    public function test_snake_case_keys(): void
    {
        foreach (array_keys(['my_key' => 1]) as $__key) {
            $this->assertMatchesRegularExpression('/^[a-z][a-z0-9]*(?:_[a-z0-9]+)*$/', $__key);
        }
    }
}
