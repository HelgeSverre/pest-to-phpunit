<?php
beforeAll(function () {
    Config::set('app.env', 'testing');
});

beforeEach(function () {
    $this->counter = 0;
});

afterEach(function () {
    $this->counter = null;
});

afterAll(function () {
    Config::set('app.env', 'production');
});

test('all hooks', function () {
    expect($this->counter)->toBe(0);
});
-----
<?php

class HookAllFourTypesTest extends \PHPUnit\Framework\TestCase
{
    protected $counter;
    protected static function setUpBeforeClass(): void
    {
        parent::setUpBeforeClass();
        Config::set('app.env', 'testing');
    }
    protected function setUp(): void
    {
        parent::setUp();
        $this->counter = 0;
    }
    protected function tearDown(): void
    {
        parent::tearDown();
        $this->counter = null;
    }
    protected static function tearDownAfterClass(): void
    {
        parent::tearDownAfterClass();
        Config::set('app.env', 'production');
    }
    public function test_all_hooks(): void
    {
        $this->assertSame(0, $this->counter);
    }
}
