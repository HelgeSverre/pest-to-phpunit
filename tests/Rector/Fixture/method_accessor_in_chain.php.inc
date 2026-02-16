<?php

test('method accessor', function () {
    expect($collection)->count()->toBe(3);
    expect($response)->getStatusCode()->toBe(200);
});

-----
<?php

class MethodAccessorInChainTest extends \PHPUnit\Framework\TestCase
{
    public function test_method_accessor(): void
    {
        $this->assertSame(3, $collection->count());
        $this->assertSame(200, $response->getStatusCode());
    }
}
