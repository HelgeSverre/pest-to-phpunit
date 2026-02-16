<?php
test('sequence check', function () {
    expect([1, 2, 3])->sequence(
        fn ($n) => $n->toBe(1),
        fn ($n) => $n->toBe(2),
        fn ($n) => $n->toBe(3),
    );
});

-----
<?php
class SequenceWithClosuresTest extends \PHPUnit\Framework\TestCase
{
    public function test_sequence_check(): void
    {
        // TODO(Pest): ->sequence() requires manual conversion to PHPUnit
    }
}
