<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Tests\Unit;

use HelgeSverre\PestToPhpUnit\Helper\NameHelper;
use PHPUnit\Framework\Attributes\DataProvider;
use PHPUnit\Framework\TestCase;

final class NameHelperTest extends TestCase
{
    /**
     * @return array<string, array{string, string, string}>
     */
    public static function descriptionToMethodNameProvider(): array
    {
        return [
            'basic with test prefix' => ['returns true', 'test', 'test_returns_true'],
            'with it prefix' => ['works well', 'it', 'test_it_works_well'],
            'already has it prefix' => ['it works', 'it', 'test_it_works'],
            'case-insensitive it detection' => ['It handles edge cases', 'it', 'test_it_handles_edge_cases'],
            'special chars' => ['has @special #chars!', 'test', 'test_has_special_chars'],
            'empty string' => ['', 'it', 'test_it'],
            'numbers' => ['returns 42 items', 'test', 'test_returns_42_items'],
            'multiple spaces' => ['has   multiple   spaces', 'test', 'test_has_multiple_spaces'],
        ];
    }

    #[DataProvider('descriptionToMethodNameProvider')]
    public function test_description_to_method_name(string $description, string $prefix, string $expected): void
    {
        $result = NameHelper::descriptionToMethodName($description, $prefix);
        $this->assertSame($expected, $result);
    }

    /**
     * @return array<string, array{string, string}>
     */
    public static function fileNameToClassNameProvider(): array
    {
        return [
            'kebab case' => ['my-feature-test.php', 'MyFeatureTest'],
            'snake case' => ['my_feature_test.php', 'MyFeatureTest'],
            'already PascalCase with Test' => ['ExampleTest.php', 'ExampleTest'],
            'no Test suffix' => ['helpers.php', 'HelpersTest'],
            'dots in name' => ['foo.bar.php', 'FooBarTest'],
            'single word ending with Test' => ['test.php', 'Test'],
        ];
    }

    #[DataProvider('fileNameToClassNameProvider')]
    public function test_file_name_to_class_name(string $fileName, string $expected): void
    {
        $result = NameHelper::fileNameToClassName($fileName);
        $this->assertSame($expected, $result);
    }
}
