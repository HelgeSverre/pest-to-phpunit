<?php
// Edge case: complex body calling method on $this->value (Laravel TestResponse style)
expect()->extend('toBeOkResponse', function () {
    $this->value->assertOk();
    return $this;
});

test('laravel response assert', function () {
    expect($response)->toBeOkResponse();
});

-----
<?php

class CustomExpectComplexMethodOnValueTest extends \PHPUnit\Framework\TestCase
{
    public function test_laravel_response_assert(): void
    {
        // TODO(Pest): Custom expectation ->toBeOkResponse() was defined via expect()->extend() and requires manual conversion
        $__pest_subject = $response;
        $__pest_subject->assertOk();
    }
}
