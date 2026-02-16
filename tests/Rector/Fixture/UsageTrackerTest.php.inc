<?php

beforeEach(function () {
    $this->tracker = new UsageTracker;
});

afterEach(function () {
    $this->tracker = null;
});

test('tracker listens to events', function () {
    $events = $this->tracker->listensTo();
    expect($events)->toContain('after_llm_response');
});

test('tracker does not listen when disabled', function () {
    $tracker = new UsageTracker(['track_llm' => false]);
    $events = $tracker->listensTo();
    expect($events)->not->toContain('after_llm_response');
});

test('tracker records and chains assertions', function () {
    $records = $this->tracker->getAll();
    expect($records)->toHaveCount(1)
        ->and($records[0]->name)->toBe('test-agent')
        ->and($records[0]->cost)->toBeGreaterThan(0);
});

test('tracker clears all records', function () {
    expect($this->tracker->getAll())->toHaveCount(1);
    $this->tracker->clear();
    expect($this->tracker->getAll())->toBeEmpty()
        ->and($this->tracker->getTotalCost())->toBe(0.0);
});

-----
<?php

class UsageTrackerTest extends \PHPUnit\Framework\TestCase
{
    protected $tracker;
    protected function setUp(): void
    {
        parent::setUp();
        $this->tracker = new UsageTracker;
    }
    protected function tearDown(): void
    {
        parent::tearDown();
        $this->tracker = null;
    }
    public function test_tracker_listens_to_events(): void
    {
        $events = $this->tracker->listensTo();
        $this->assertContains('after_llm_response', $events);
    }
    public function test_tracker_does_not_listen_when_disabled(): void
    {
        $tracker = new UsageTracker(['track_llm' => false]);
        $events = $tracker->listensTo();
        $this->assertNotContains('after_llm_response', $events);
    }
    public function test_tracker_records_and_chains_assertions(): void
    {
        $records = $this->tracker->getAll();
        $this->assertCount(1, $records);
        $this->assertSame('test-agent', $records[0]->name);
        $this->assertGreaterThan(0, $records[0]->cost);
    }
    public function test_tracker_clears_all_records(): void
    {
        $this->assertCount(1, $this->tracker->getAll());
        $this->tracker->clear();
        $this->assertEmpty($this->tracker->getAll());
        $this->assertSame(0.0, $this->tracker->getTotalCost());
    }
}
