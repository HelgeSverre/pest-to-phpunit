<?php
describe('Auth', function () {
    it('logs in', function () {
        expect(true)->toBeTrue();
    });
});

describe('Profile', function () {
    it('updates name', function () {
        expect(true)->toBeTrue();
    });
});
-----
<?php

class DescribeMultipleSameLevelTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_auth_logs_in(): void
    {
        $this->assertTrue(true);
    }
    public function test_it_profile_updates_name(): void
    {
        $this->assertTrue(true);
    }
}
