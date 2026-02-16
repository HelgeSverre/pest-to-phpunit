<?php
beforeEach(function () {
    if (env('USE_REAL_DB')) {
        $this->db = new RealDB();
    } else {
        $this->db = new FakeDB();
    }
    $this->initialized = true;
});

test('has db', function () {
    expect($this->db)->not->toBeNull();
    expect($this->initialized)->toBeTrue();
});
-----
<?php

class PropertyFromNestedIfTest extends \PHPUnit\Framework\TestCase
{
    protected $db;
    protected $initialized;
    protected function setUp(): void
    {
        parent::setUp();
        if (env('USE_REAL_DB')) {
            $this->db = new RealDB();
        } else {
            $this->db = new FakeDB();
        }
        $this->initialized = true;
    }
    public function test_has_db(): void
    {
        $this->assertNotNull($this->db);
        $this->assertTrue($this->initialized);
    }
}
