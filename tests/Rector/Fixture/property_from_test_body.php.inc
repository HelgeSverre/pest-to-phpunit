<?php
test('assigns property', function () {
    $this->user = createUser();
    expect($this->user)->not->toBeNull();
});
-----
<?php
class PropertyFromTestBodyTest extends \PHPUnit\Framework\TestCase
{
    protected $user;
    public function test_assigns_property(): void
    {
        $this->user = createUser();
        $this->assertNotNull($this->user);
    }
}
