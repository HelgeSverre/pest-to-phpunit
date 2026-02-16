<?php
// Edge case: body returns a non-chain expression (not $this->toX and not expect())
expect()->extend('toBeComputed', function () {
    return (bool) $this->value;
});

test('return non chain', function () {
    expect(1)->toBeComputed();
});

-----
<?php

class CustomExpectReturnNonChainTest extends \PHPUnit\Framework\TestCase
{
    public function test_return_non_chain(): void
    {
        // TODO(Pest): Custom expectation ->toBeComputed() was defined via expect()->extend() and requires manual conversion
        $__pest_subject = 1;
    }
}
