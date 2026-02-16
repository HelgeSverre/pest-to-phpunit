<?php

test('cleans up after', function () {
    $file = tempnam('/tmp', 'test');
    expect($file)->toBeString();
})->after(function () {
    unlink($file);
});

-----
<?php

class AfterHookTest extends \PHPUnit\Framework\TestCase
{
    public function test_cleans_up_after(): void
    {
        try {
            $file = tempnam('/tmp', 'test');
            $this->assertIsString($file);
        } finally {
            unlink($file);
        }
    }
}
