<?php
afterAll(function () {
    Cache::flush();
    DB::rollBack();
});

test('cached value', function () {
    expect(true)->toBeTrue();
});
-----
<?php

class HookAfterallWithBodyTest extends \PHPUnit\Framework\TestCase
{
    protected static function tearDownAfterClass(): void
    {
        parent::tearDownAfterClass();
        Cache::flush();
        DB::rollBack();
    }
    public function test_cached_value(): void
    {
        $this->assertTrue(true);
    }
}
