<?php

test('each modifier', function () {
    expect($items)->each->toBeString();
});

-----
<?php

class EachModifierTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_modifier(): void
    {
        foreach ($items as $__each_item) {
            $this->assertIsString($__each_item);
        }
    }
}
