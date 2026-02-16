<?php
beforeEach(function () {
    $this->x = 1;
});

afterEach(function () {
    $this->x = null;
});
-----
<?php

class OnlyHooksNoTestsTest extends \PHPUnit\Framework\TestCase
{
    protected $x;
    protected function setUp(): void
    {
        parent::setUp();
        $this->x = 1;
    }
    protected function tearDown(): void
    {
        parent::tearDown();
        $this->x = null;
    }
}
