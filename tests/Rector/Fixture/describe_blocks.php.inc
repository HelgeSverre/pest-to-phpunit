<?php

describe('Auth', function () {
    it('logs in', function () {
        expect(true)->toBeTrue();
    });

    it('logs out', function () {
        expect(true)->toBeTrue();
    });
});

-----
<?php

class DescribeBlocksTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_auth_logs_in(): void
    {
        $this->assertTrue(true);
    }
    public function test_it_auth_logs_out(): void
    {
        $this->assertTrue(true);
    }
}
