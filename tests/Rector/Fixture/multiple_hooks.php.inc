<?php
beforeEach(function () {
    $this->db = new Database();
});

beforeEach(function () {
    $this->db->beginTransaction();
});

afterEach(function () {
    $this->db->rollBack();
});

test('query works', function () {
    expect($this->db)->not->toBeNull();
});

-----
<?php

class MultipleHooksTest extends \PHPUnit\Framework\TestCase
{
    protected $db;
    protected function setUp(): void
    {
        parent::setUp();
        $this->db = new Database();
    }
    protected function setUp(): void
    {
        parent::setUp();
        $this->db->beginTransaction();
    }
    protected function tearDown(): void
    {
        parent::tearDown();
        $this->db->rollBack();
    }
    public function test_query_works(): void
    {
        $this->assertNotNull($this->db);
    }
}
