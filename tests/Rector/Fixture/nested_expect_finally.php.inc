<?php

test('finally expect', function () {
    $result = null;
    try {
        $result = riskyOperation();
        expect($result)->toBeString();
    } finally {
        expect($result)->not->toBeNull();
    }
});

-----
<?php

class NestedExpectFinallyTest extends \PHPUnit\Framework\TestCase
{
    public function test_finally_expect(): void
    {
        $result = null;
        try {
            $result = riskyOperation();
            $this->assertIsString($result);
        } finally {
            $this->assertNotNull($result);
        }
    }
}
