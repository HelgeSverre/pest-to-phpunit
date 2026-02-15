<?php

use HelgeSverre\ReceiptScanner\TextUtils;

it('normalizes whitespace', function () {
    $input = '   Hello   World!   ';
    $expectedOutput = 'Hello World!';

    $output = TextUtils::normalizeWhitespace($input);

    expect($output)->toBe($expectedOutput);
});

it('removes unwanted elements from HTML', function () {
    $html = '
        <html>
            <head>
                <title>Test Page</title>
                <script>alert("Hello!");</script>
                <style>body { background: red; }</style>
            </head>
            <body>
                <p>This is a test page.</p>
                <script>alert("World!");</script>
            </body>
        </html>
    ';
    $expectedOutput = 'This is a test page.';

    $output = TextUtils::cleanHtml($html);

    expect($output)->toBe($expectedOutput);
});

it('keeps whitespaces in HTML', function () {
    $html = '
        <p>Hello   World!</p>
    ';

    $output = TextUtils::cleanHtml($html, elementsToRemove: [], normalizeWhitespace: true);

    expect($output)->toBe('Hello World!');
});

it('ignores the head tag when cleaning html', function () {
    $html = '
        <html>
            <head>
                <title>Test Page</title>
            </head>
            <body>
                <p>This is a test page.</p>
            </body>
        </html>
    ';
    $expectedOutput = 'This is a test page.';

    $output = TextUtils::cleanHtml($html);

    expect($output)->toBe($expectedOutput);
});

it('removes comments from HTML', function () {
    $html = '
        <html>
            <body>
                <p>This is a test</p>
                <!-- Another comment -->
                <p>page.</p>
            </body>
        </html>
    ';
    $expectedOutput = 'This is a test page.';

    $output = TextUtils::cleanHtml($html);

    expect($output)->toBe($expectedOutput);
});

-----
<?php

use HelgeSverre\ReceiptScanner\TextUtils;
class TextUtilsTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_normalizes_whitespace(): void
    {
        $input = '   Hello   World!   ';
        $expectedOutput = 'Hello World!';
        $output = TextUtils::normalizeWhitespace($input);
        $this->assertSame($expectedOutput, $output);
    }
    public function test_it_removes_unwanted_elements_from_html(): void
    {
        $html = '
        <html>
            <head>
                <title>Test Page</title>
                <script>alert("Hello!");</script>
                <style>body { background: red; }</style>
            </head>
            <body>
                <p>This is a test page.</p>
                <script>alert("World!");</script>
            </body>
        </html>
    ';
        $expectedOutput = 'This is a test page.';
        $output = TextUtils::cleanHtml($html);
        $this->assertSame($expectedOutput, $output);
    }
    public function test_it_keeps_whitespaces_in_html(): void
    {
        $html = '
        <p>Hello   World!</p>
    ';
        $output = TextUtils::cleanHtml($html, elementsToRemove: [], normalizeWhitespace: true);
        $this->assertSame('Hello World!', $output);
    }
    public function test_it_ignores_the_head_tag_when_cleaning_html(): void
    {
        $html = '
        <html>
            <head>
                <title>Test Page</title>
            </head>
            <body>
                <p>This is a test page.</p>
            </body>
        </html>
    ';
        $expectedOutput = 'This is a test page.';
        $output = TextUtils::cleanHtml($html);
        $this->assertSame($expectedOutput, $output);
    }
    public function test_it_removes_comments_from_html(): void
    {
        $html = '
        <html>
            <body>
                <p>This is a test</p>
                <!-- Another comment -->
                <p>page.</p>
            </body>
        </html>
    ';
        $expectedOutput = 'This is a test page.';
        $output = TextUtils::cleanHtml($html);
        $this->assertSame($expectedOutput, $output);
    }
}
