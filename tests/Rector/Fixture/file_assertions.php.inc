<?php

test('file assertions', function () {
    expect('/tmp/test.txt')->toBeFile();
    expect('/tmp/test.txt')->toBeReadableFile();
    expect('/tmp/test.txt')->toBeWritableFile();
    expect('/tmp')->toBeDirectory();
    expect('/tmp')->toBeReadableDirectory();
    expect('/tmp')->toBeWritableDirectory();
});

-----
<?php

class FileAssertionsTest extends \PHPUnit\Framework\TestCase
{
    public function test_file_assertions(): void
    {
        $this->assertFileExists('/tmp/test.txt');
        $this->assertFileIsReadable('/tmp/test.txt');
        $this->assertFileIsWritable('/tmp/test.txt');
        $this->assertDirectoryExists('/tmp');
        $this->assertDirectoryIsReadable('/tmp');
        $this->assertDirectoryIsWritable('/tmp');
    }
}
