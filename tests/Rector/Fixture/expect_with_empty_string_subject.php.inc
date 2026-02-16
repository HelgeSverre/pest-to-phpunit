<?php
test('empty string subject', function () {
    expect('')->toBeEmpty();
    expect('')->toBeString();
    expect('')->toBeFalsy();
});
-----
<?php
class ExpectWithEmptyStringSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_empty_string_subject(): void
    {
        $this->assertEmpty('');
        $this->assertIsString('');
        $this->assertEmpty('');
    }
}
