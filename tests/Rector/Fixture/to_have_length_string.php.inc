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
        $this->assertCount(5, 'hello');
    }
}
