<?php
beforeEach(function () {
    $this->result = calculateSomething();
    expect($this->result)->not->toBeNull();
});

test('uses precondition', function () {
    expect($this->result)->toBeInt();
});
-----
<?php

class HookWithExpectInBodyTest extends \PHPUnit\Framework\TestCase
{
    protected $result;
    protected function setUp(): void
    {
        parent::setUp();
        $this->result = calculateSomething();
        $this->assertNotNull($this->result);
    }
    public function test_uses_precondition(): void
    {
        $this->assertIsInt($this->result);
    }
}
