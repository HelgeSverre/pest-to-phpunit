<?php
test('negated case formats', function () {
    expect('HELLO_WORLD')->not->toBeSnakeCase();
    expect('hello_world')->not->toBeCamelCase();
    expect('helloWorld')->not->toBeStudlyCase();
    expect('HelloWorld')->not->toBeKebabCase();
});
-----
<?php
class NegatedCaseFormatsTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_case_formats(): void
    {
        $this->assertDoesNotMatchRegularExpression('/^[a-z][a-z0-9]*(?:_[a-z0-9]+)*$/', 'HELLO_WORLD');
        $this->assertDoesNotMatchRegularExpression('/^[a-z][a-zA-Z0-9]*$/', 'hello_world');
        $this->assertDoesNotMatchRegularExpression('/^[A-Z][a-zA-Z0-9]*$/', 'helloWorld');
        $this->assertDoesNotMatchRegularExpression('/^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$/', 'HelloWorld');
    }
}
