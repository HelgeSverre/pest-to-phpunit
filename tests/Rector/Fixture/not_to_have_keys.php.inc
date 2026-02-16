<?php

test('not have keys', function () {
    expect($arr)->not->toHaveKeys(['secret', 'password']);
});

-----
<?php

class NotToHaveKeysTest extends \PHPUnit\Framework\TestCase
{
    public function test_not_have_keys(): void
    {
        $this->assertArrayNotHasKey('secret', $arr);
        $this->assertArrayNotHasKey('password', $arr);
    }
}
