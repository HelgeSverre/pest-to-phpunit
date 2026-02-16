<?php
beforeEach(function () {
    $this->name = 'John';
    $this->age = 30;
    $this->active = true;
});

test('has properties', function () {
    expect($this->name)->toBe('John');
    expect($this->age)->toBe(30);
    expect($this->active)->toBeTrue();
});

-----
<?php

class HookWithMultiplePropertiesTest extends \PHPUnit\Framework\TestCase
{
    protected $name;
    protected $age;
    protected $active;
    protected function setUp(): void
    {
        parent::setUp();
        $this->name = 'John';
        $this->age = 30;
        $this->active = true;
    }
    public function test_has_properties(): void
    {
        $this->assertSame('John', $this->name);
        $this->assertSame(30, $this->age);
        $this->assertTrue($this->active);
    }
}
