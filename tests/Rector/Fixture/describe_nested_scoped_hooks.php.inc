<?php

describe('Outer', function () {
    beforeEach(function () {
        $this->outer = 'outer';
    });

    describe('Inner', function () {
        beforeEach(function () {
            $this->inner = 'inner';
        });

        test('has both', function () {
            expect($this->outer)->toBe('outer');
            expect($this->inner)->toBe('inner');
        });
    });
});

-----
<?php

class DescribeNestedScopedHooksTest extends \PHPUnit\Framework\TestCase
{
    protected $outer;
    protected $inner;
    public function test_outer_inner_has_both(): void
    {
        $this->outer = 'outer';
        $this->inner = 'inner';
        $this->assertSame('outer', $this->outer);
        $this->assertSame('inner', $this->inner);
    }
}
