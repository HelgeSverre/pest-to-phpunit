<?php
// Source: spatie/laravel-medialibrary tests/Pest.php
expect()->extend('toHaveExtension', function (string $expectedExtension) {
    $actualExtension = pathinfo($this->value, PATHINFO_EXTENSION);
    expect($actualExtension)->toEqual($expectedExtension);
});

test('file has jpg extension', function () {
    expect('photo.jpg')->toHaveExtension('jpg');
});

-----
<?php

class CustomExpectFileExtensionTest extends \PHPUnit\Framework\TestCase
{
    public function test_file_has_jpg_extension(): void
    {
        $actualExtension = pathinfo('photo.jpg', PATHINFO_EXTENSION);
        $this->assertEquals('jpg', $actualExtension);
    }
}
