<?php

describe('Auth', function () {
    it('logs in', function () {
        expect(true)->toBeTrue();
    });
});

-----
<?php

class ReadmeDescribeTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_auth_logs_in(): void
    {
        $this->assertTrue(true);
    }
}
