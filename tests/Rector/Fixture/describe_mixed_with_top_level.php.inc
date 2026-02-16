<?php
test('standalone test', function () {
    expect(true)->toBeTrue();
});

describe('Group', function () {
    it('grouped test', function () {
        expect(true)->toBeTrue();
    });
});

test('another standalone', function () {
    expect(true)->toBeTrue();
});
-----
<?php

class DescribeMixedWithTopLevelTest extends \PHPUnit\Framework\TestCase
{
    public function test_standalone_test(): void
    {
        $this->assertTrue(true);
    }
    public function test_it_group_grouped_test(): void
    {
        $this->assertTrue(true);
    }
    public function test_another_standalone(): void
    {
        $this->assertTrue(true);
    }
}
