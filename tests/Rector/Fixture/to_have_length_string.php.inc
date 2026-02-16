<?php

test('string length', function () {
    expect('hello')->toHaveLength(5);
});

-----
<?php

class ToHaveLengthStringTest extends \PHPUnit\Framework\TestCase
{
    public function test_string_length(): void
    {
        $this->assertSame(5, is_string('hello') ? strlen('hello') : count('hello'));
    }
}
