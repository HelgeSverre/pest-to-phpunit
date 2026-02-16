<?php
beforeEach(fn() => $this->x = new User());

test('has user', function () {
    expect($this->x)->not->toBeNull();
});

-----
<?php

class HookArrowFunctionTest extends \PHPUnit\Framework\TestCase
{
    protected $x;
    protected function setUp(): void
    {
        parent::setUp();
        $this->x = new User();
    }
    public function test_has_user(): void
    {
        $this->assertNotNull($this->x);
    }
}
