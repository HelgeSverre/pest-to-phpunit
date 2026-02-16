<?php
beforeEach(function () {
    $this->service = new Service();
    $this->service->boot();
});

test('service booted', function () {
    expect($this->service)->not->toBeNull();
});

-----
<?php

class HookWithMethodCallsTest extends \PHPUnit\Framework\TestCase
{
    protected $service;
    protected function setUp(): void
    {
        parent::setUp();
        $this->service = new Service();
        $this->service->boot();
    }
    public function test_service_booted(): void
    {
        $this->assertNotNull($this->service);
    }
}
