<?php

test('each not combined', function () {
    expect($items)->each->not->toBeNull();
    expect($values)->each->not->toBe(0);
});

-----
<?php

class EachNotCombinedTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_not_combined(): void
    {
        foreach ($items as $__pest_each_item) {
            $this->assertNotNull($__pest_each_item);
        }
        foreach ($values as $__pest_each_item) {
            $this->assertNotSame(0, $__pest_each_item);
        }
    }
}
