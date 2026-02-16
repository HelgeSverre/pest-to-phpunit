<?php
test('skipped no reason', function () {
    expect(true)->toBeTrue();
})->skip();

-----
<?php
class SkipNoArgsTest extends \PHPUnit\Framework\TestCase
{
    public function test_skipped_no_reason(): void
    {
        $this->markTestSkipped('Skipped');
    }
}

