<?php
// Edge case: $this->value used with array dim fetch
expect()->extend('toHaveIdInt', function () {
    expect($this->value['id'])->toBeInt();
});

test('array access on this value', function () {
    expect(['id' => 123])->toHaveIdInt();
});

-----
<?php

class CustomExpectThisValueArrayAccessTest extends \PHPUnit\Framework\TestCase
{
    public function test_array_access_on_this_value(): void
    {
        $this->assertIsInt(['id' => 123]['id']);
    }
}
