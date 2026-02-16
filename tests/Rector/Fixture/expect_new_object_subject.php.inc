<?php
test('new object as subject', function () {
    expect(new DateTime())->toBeInstanceOf(DateTime::class);
});

-----
<?php
class ExpectNewObjectSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_new_object_as_subject(): void
    {
        $this->assertInstanceOf(DateTime::class, new DateTime());
    }
}
