<?php
test('case format checks', function () {
    expect('hello_world')->toBeSnakeCase();
    expect('hello-world')->toBeKebabCase();
    expect('helloWorld')->toBeCamelCase();
    expect('HelloWorld')->toBeStudlyCase();
});

-----
<?php
class ToBeCaseFormatsTest extends \PHPUnit\Framework\TestCase
{
    public function test_case_format_checks(): void
    {
        $this->assertMatchesRegularExpression('/^[a-z][a-z0-9]*(?:_[a-z0-9]+)*$/', 'hello_world');
        $this->assertMatchesRegularExpression('/^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$/', 'hello-world');
        $this->assertMatchesRegularExpression('/^[a-z][a-zA-Z0-9]*$/', 'helloWorld');
        $this->assertMatchesRegularExpression('/^[A-Z][a-zA-Z0-9]*$/', 'HelloWorld');
    }
}
