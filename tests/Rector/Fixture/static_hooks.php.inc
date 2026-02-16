<?php
beforeAll(function () {
    self::$connection = new PDO('sqlite::memory:');
});

afterAll(function () {
    self::$connection = null;
});

test('has connection', function () {
    expect(true)->toBeTrue();
});

-----
<?php

class StaticHooksTest extends \PHPUnit\Framework\TestCase
{
    protected static function setUpBeforeClass(): void
    {
        parent::setUpBeforeClass();
        self::$connection = new PDO('sqlite::memory:');
    }
    protected static function tearDownAfterClass(): void
    {
        parent::tearDownAfterClass();
        self::$connection = null;
    }
    public function test_has_connection(): void
    {
        $this->assertTrue(true);
    }
}
