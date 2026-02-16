<?php
test('has snake case keys', function () {
    expect($data)->toHaveSnakeCaseKeys();
});

test('has kebab case keys', function () {
    expect($data)->toHaveKebabCaseKeys();
});

test('has camel case keys', function () {
    expect($data)->toHaveCamelCaseKeys();
});

test('has studly case keys', function () {
    expect($data)->toHaveStudlyCaseKeys();
});

-----
<?php

class ToHaveCaseKeysTest extends \PHPUnit\Framework\TestCase
{
    public function test_has_snake_case_keys(): void
    {
        foreach (array_keys($data) as $__key) {
            $this->assertMatchesRegularExpression('/^[a-z][a-z0-9]*(?:_[a-z0-9]+)*$/', $__key);
        }
    }
    public function test_has_kebab_case_keys(): void
    {
        foreach (array_keys($data) as $__key) {
            $this->assertMatchesRegularExpression('/^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$/', $__key);
        }
    }
    public function test_has_camel_case_keys(): void
    {
        foreach (array_keys($data) as $__key) {
            $this->assertMatchesRegularExpression('/^[a-z][a-zA-Z0-9]*$/', $__key);
        }
    }
    public function test_has_studly_case_keys(): void
    {
        foreach (array_keys($data) as $__key) {
            $this->assertMatchesRegularExpression('/^[A-Z][a-zA-Z0-9]*$/', $__key);
        }
    }
}