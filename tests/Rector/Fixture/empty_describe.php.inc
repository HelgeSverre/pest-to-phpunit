<?php
describe('empty group', function () {
    // nothing here
});

test('standalone', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class EmptyDescribeTest extends \PHPUnit\Framework\TestCase
{
    public function test_standalone(): void
    {
        $this->assertTrue(true);
    }
}
