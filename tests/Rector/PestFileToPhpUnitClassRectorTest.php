<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Tests\Rector;

use Iterator;
use PhpParser\Error as PhpParserError;
use PhpParser\ParserFactory;
use PHPUnit\Framework\Attributes\DataProvider;
use Rector\Testing\PHPUnit\AbstractRectorTestCase;

final class PestFileToPhpUnitClassRectorTest extends AbstractRectorTestCase
{
    #[DataProvider('provideData')]
    public function test(string $filePath): void
    {
        $this->doTestFile($filePath);
        $this->assertFixtureOutputIsValidPhp($filePath);
        $this->assertNoPestCallsRemain($filePath);
    }

    public static function provideData(): Iterator
    {
        return self::yieldFilesFromDirectory(__DIR__ . '/Fixture');
    }

    public function provideConfigFilePath(): string
    {
        return __DIR__ . '/config/configured_rule.php';
    }

    private function assertFixtureOutputIsValidPhp(string $filePath): void
    {
        $content = file_get_contents($filePath);
        $parts = explode("-----\n", $content, 2);

        if (count($parts) < 2) {
            return;
        }

        $expectedOutput = trim($parts[1]);
        if ($expectedOutput === '') {
            return;
        }

        $parser = (new ParserFactory())->createForNewestSupportedVersion();

        try {
            $stmts = $parser->parse($expectedOutput);
            $this->assertNotNull($stmts, "Failed to parse expected output of fixture: " . basename($filePath));
        } catch (PhpParserError $e) {
            $this->fail("Expected output of fixture " . basename($filePath) . " is not valid PHP: " . $e->getMessage());
        }
    }

    private function assertNoPestCallsRemain(string $filePath): void
    {
        $content = file_get_contents($filePath);
        $parts = explode("-----\n", $content, 2);

        if (count($parts) < 2) {
            return;
        }

        $expectedOutput = trim($parts[1]);
        if ($expectedOutput === '') {
            return;
        }

        // Strip comments so TODO comments don't trigger false positives
        $withoutComments = preg_replace('#//.*$#m', '', $expectedOutput);
        $withoutComments = preg_replace('#/\*.*?\*/#s', '', $withoutComments);
        // Strip string literals to avoid matching inside strings
        $withoutComments = preg_replace('#\'[^\']*\'#', "''", $withoutComments);
        $withoutComments = preg_replace('#"[^"]*"#', '""', $withoutComments);

        // Match only bare function calls — not method calls (->test(), ::test())
        // and not substrings of identifiers (test_something, expectException)
        $pestFunctions = ['expect', 'test', 'it', 'describe', 'beforeEach', 'afterEach', 'beforeAll', 'afterAll'];
        foreach ($pestFunctions as $func) {
            // Negative lookbehind: not preceded by ->, ::, or word char (rules out method calls and longer identifiers)
            $pattern = '/(?<!->)(?<!::)(?<![a-zA-Z0-9_])' . preg_quote($func, '/') . '\s*\(/';
            $this->assertDoesNotMatchRegularExpression(
                $pattern,
                $withoutComments,
                "Expected output of " . basename($filePath) . " still contains Pest call: {$func}()"
            );
        }
    }
}
