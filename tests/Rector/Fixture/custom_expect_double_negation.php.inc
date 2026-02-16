<?php
// Edge case: custom body uses ->not-> and call site also uses ->not->
expect()->extend('toBeNotNullCustom', function () {
    return $this->not->toBeNull();
});

test('double negation', function () {
    expect(null)->not->toBeNotNullCustom();
});

-----
<?php

class CustomExpectDoubleNegationTest extends \PHPUnit\Framework\TestCase
{
    public function test_double_negation(): void
    {
        $this->assertNull(null);
    }
}
