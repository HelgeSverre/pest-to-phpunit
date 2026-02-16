<?php
test('each has length', function () {
    expect(['ab', 'cd', 'ef'])->each->toHaveLength(2);
});
-----
<?php
class EachToHaveLengthTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_has_length(): void
    {
        foreach (['ab', 'cd', 'ef'] as $__pest_each_item) {
            $this->assertSame(2, is_string($__pest_each_item) ? strlen($__pest_each_item) : count($__pest_each_item));
        }
    }
}
