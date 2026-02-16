<?php
test('matches object', function () {
    expect($obj)->toMatchObject(['name' => 'John', 'age' => 30]);
});

-----
<?php
class ToMatchObjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_matches_object(): void
    {
        $this->assertEquals(['name' => 'John', 'age' => 30], $obj);
    }
}
