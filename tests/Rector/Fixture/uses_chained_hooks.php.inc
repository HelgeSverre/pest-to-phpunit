<?php

uses(SomeTrait::class)->beforeEach(function () {
    $this->service = new Service;
})->afterEach(function () {
    $this->service = null;
})->group('unit');

test('it works', function () {
    expect($this->service)->toBeInstanceOf(Service::class);
});

-----
<?php

#[\PHPUnit\Framework\Attributes\Group('unit')]
class UsesChainedHooksTest extends \PHPUnit\Framework\TestCase
{
    use SomeTrait;
    protected $service;
    protected function setUp(): void
    {
        parent::setUp();
        $this->service = new Service;
    }
    protected function tearDown(): void
    {
        parent::tearDown();
        $this->service = null;
    }
    public function test_it_works(): void
    {
        $this->assertInstanceOf(Service::class, $this->service);
    }
}
