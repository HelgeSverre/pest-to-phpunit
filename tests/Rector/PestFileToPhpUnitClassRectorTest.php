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
}
