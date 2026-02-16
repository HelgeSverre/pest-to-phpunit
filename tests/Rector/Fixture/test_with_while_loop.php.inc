<?php
test('with while loop', function () {
    $i = 0;
    $results = [];
    while ($i < 3) {
        $results[] = $i * 2;
        $i++;
    }
    expect($results)->toBe([0, 2, 4]);
});
-----
<?php
class TestWithWhileLoopTest extends \PHPUnit\Framework\TestCase
{
    public function test_with_while_loop(): void
    {
        $i = 0;
        $results = [];
        while ($i < 3) {
            $results[] = $i * 2;
            $i++;
        }
        $this->assertSame([0, 2, 4], $results);
    }
}
