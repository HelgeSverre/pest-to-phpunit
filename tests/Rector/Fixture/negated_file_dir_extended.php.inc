<?php
test('negated file dir extended', function () {
    expect('/tmp/x')->not->toBeReadableDirectory();
    expect('/tmp/x')->not->toBeWritableDirectory();
});
-----
<?php
class NegatedFileDirExtendedTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_file_dir_extended(): void
    {
        $this->assertDirectoryIsNotReadable('/tmp/x');
        $this->assertDirectoryIsNotWritable('/tmp/x');
    }
}
