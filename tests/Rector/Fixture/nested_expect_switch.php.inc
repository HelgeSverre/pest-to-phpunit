<?php

test('switch expect', function () {
    $type = getType();
    switch ($type) {
        case 'string':
            expect($type)->toBe('string');
            break;
        case 'int':
            expect($type)->toBe('int');
            break;
        default:
            expect($type)->toBeString();
    }
});

-----
<?php

class NestedExpectSwitchTest extends \PHPUnit\Framework\TestCase
{
    public function test_switch_expect(): void
    {
        $type = getType();
        switch ($type) {
            case 'string':
                $this->assertSame('string', $type);
                break;
            case 'int':
                $this->assertSame('int', $type);
                break;
            default:
                $this->assertIsString($type);
        }
    }
}
