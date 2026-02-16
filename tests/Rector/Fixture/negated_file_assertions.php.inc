<?php
test('negated file checks', function () {
    expect('/nonexistent')->not->toBeFile();
    expect('/nonexistent')->not->toBeDirectory();
});
-----
<?php
class NegatedFileAssertionsTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_file_checks(): void
    {
        $this->assertFileDoesNotExist('/nonexistent');
        $this->assertDirectoryDoesNotExist('/nonexistent');
    }
}
