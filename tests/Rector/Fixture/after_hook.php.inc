<?php
test('cleans up after', function () {
    $resource = acquire();
    expect($resource)->toBeInstanceOf(Resource::class);
})->after(function () {
    cleanup();
});

-----
<?php
class AfterHookTest extends \PHPUnit\Framework\TestCase
{
    public function test_cleans_up_after(): void
    {
        try {
            $resource = acquire();
            $this->assertInstanceOf(Resource::class, $resource);
        } finally {
            cleanup();
        }
    }
}
