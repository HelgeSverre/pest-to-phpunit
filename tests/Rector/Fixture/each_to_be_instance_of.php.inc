<?php
test('each instanceof', function () {
    expect($users)->each->toBeInstanceOf(User::class);
});

-----
<?php
class EachToBeInstanceOfTest extends \PHPUnit\Framework\TestCase
{
    public function test_each_instanceof(): void
    {
        foreach ($users as $__each_item) {
            $this->assertInstanceOf(User::class, $__each_item);
        }
    }
}
