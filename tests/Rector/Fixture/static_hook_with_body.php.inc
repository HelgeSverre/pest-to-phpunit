<?php
beforeAll(function () {
    Config::set('app.debug', true);
});

afterAll(function () {
    Config::set('app.debug', false);
});

test('debug mode', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class StaticHookWithBodyTest extends \PHPUnit\Framework\TestCase
{
    protected static function setUpBeforeClass(): void
    {
        parent::setUpBeforeClass();
        Config::set('app.debug', true);
    }
    protected static function tearDownAfterClass(): void
    {
        parent::tearDownAfterClass();
        Config::set('app.debug', false);
    }
    public function test_debug_mode(): void
    {
        $this->assertTrue(true);
    }
}
