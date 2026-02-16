<?php
// Edge case: $this->value used with property fetch
expect()->extend('toHaveIntId', function () {
    return expect($this->value->id)->toBeInt();
});

test('object property on this value', function () {
    $user = (object) ['id' => 123];
    expect($user)->toHaveIntId();
});

-----
<?php

class CustomExpectThisValuePropertyTest extends \PHPUnit\Framework\TestCase
{
    public function test_object_property_on_this_value(): void
    {
        $user = (object) ['id' => 123];
        $this->assertIsInt($user->id);
    }
}
