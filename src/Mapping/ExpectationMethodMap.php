<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Mapping;

final class ExpectationMethodMap
{
    /**
     * Maps Pest expectation methods to PHPUnit assertion methods.
     * Format: 'pestMethod' => ['assertMethod', argOrder]
     * argOrder: 'expected_actual' means assert($expected, $actual), 'actual_only' means assert($actual)
     *
     * @var array<string, array{string, string}>
     */
    public const MAP = [
        'toBe' => ['assertSame', 'expected_actual'],
        'toEqual' => ['assertEquals', 'expected_actual'],
        'toBeNull' => ['assertNull', 'actual_only'],
        'toBeTrue' => ['assertTrue', 'actual_only'],
        'toBeFalse' => ['assertFalse', 'actual_only'],
        'toBeEmpty' => ['assertEmpty', 'actual_only'],
        'toContain' => ['assertContains', 'expected_actual'],
        'toHaveCount' => ['assertCount', 'expected_actual'],
        'toBeInstanceOf' => ['assertInstanceOf', 'expected_actual'],
        'toMatch' => ['assertMatchesRegularExpression', 'expected_actual'],
        'toStartWith' => ['assertStringStartsWith', 'expected_actual'],
        'toEndWith' => ['assertStringEndsWith', 'expected_actual'],
        'toBeString' => ['assertIsString', 'actual_only'],
        'toBeInt' => ['assertIsInt', 'actual_only'],
        'toBeFloat' => ['assertIsFloat', 'actual_only'],
        'toBeArray' => ['assertIsArray', 'actual_only'],
        'toBeBool' => ['assertIsBool', 'actual_only'],
        'toBeCallable' => ['assertIsCallable', 'actual_only'],
        'toBeIterable' => ['assertIsIterable', 'actual_only'],
        'toBeNumeric' => ['assertIsNumeric', 'actual_only'],
        'toBeObject' => ['assertIsObject', 'actual_only'],
        'toBeResource' => ['assertIsResource', 'actual_only'],
        'toBeScalar' => ['assertIsScalar', 'actual_only'],
        'toBeGreaterThan' => ['assertGreaterThan', 'expected_actual'],
        'toBeGreaterThanOrEqual' => ['assertGreaterThanOrEqual', 'expected_actual'],
        'toBeLessThan' => ['assertLessThan', 'expected_actual'],
        'toBeLessThanOrEqual' => ['assertLessThanOrEqual', 'expected_actual'],
        'toHaveKey' => ['assertArrayHasKey', 'expected_actual'],
        'toEqualWithDelta' => ['assertEqualsWithDelta', 'expected_actual'],
        'toEqualCanonicalizing' => ['assertEqualsCanonicalizing', 'expected_actual'],
        'toBeFile' => ['assertFileExists', 'actual_only'],
        'toBeReadableFile' => ['assertFileIsReadable', 'actual_only'],
        'toBeWritableFile' => ['assertFileIsWritable', 'actual_only'],
        'toBeDirectory' => ['assertDirectoryExists', 'actual_only'],
        'toBeReadableDirectory' => ['assertDirectoryIsReadable', 'actual_only'],
        'toBeWritableDirectory' => ['assertDirectoryIsWritable', 'actual_only'],
        'toBeNan' => ['assertNan', 'actual_only'],
        'toBeFinite' => ['assertIsFinite', 'actual_only'],
        'toBeInfinite' => ['assertIsInfinite', 'actual_only'],
        'toBeJson' => ['assertJson', 'actual_only'],
        'toHaveLength' => ['assertCount', 'expected_actual'],
        'toBeList' => ['assertIsList', 'actual_only'],
        'toHaveSameSize' => ['assertSameSize', 'expected_actual'],
        'toContainEqual' => ['assertContainsEquals', 'expected_actual'],
    ];

    /**
     * Maps for negated assertions (not->toX).
     *
     * @var array<string, string>
     */
    public const NEGATED_MAP = [
        'assertSame' => 'assertNotSame',
        'assertEquals' => 'assertNotEquals',
        'assertNull' => 'assertNotNull',
        'assertTrue' => 'assertNotTrue',
        'assertFalse' => 'assertNotFalse',
        'assertEmpty' => 'assertNotEmpty',
        'assertContains' => 'assertNotContains',
        'assertCount' => 'assertNotCount',
        'assertInstanceOf' => 'assertNotInstanceOf',
        'assertMatchesRegularExpression' => 'assertDoesNotMatchRegularExpression',
        'assertStringStartsWith' => 'assertStringStartsNotWith',
        'assertStringEndsWith' => 'assertStringEndsNotWith',
        'assertIsString' => 'assertIsNotString',
        'assertIsInt' => 'assertIsNotInt',
        'assertIsFloat' => 'assertIsNotFloat',
        'assertIsArray' => 'assertIsNotArray',
        'assertIsBool' => 'assertIsNotBool',
        'assertIsCallable' => 'assertIsNotCallable',
        'assertIsIterable' => 'assertIsNotIterable',
        'assertIsNumeric' => 'assertIsNotNumeric',
        'assertIsObject' => 'assertIsNotObject',
        'assertIsResource' => 'assertIsNotResource',
        'assertIsScalar' => 'assertIsNotScalar',
        'assertGreaterThan' => 'assertLessThanOrEqual',
        'assertGreaterThanOrEqual' => 'assertLessThan',
        'assertLessThan' => 'assertGreaterThanOrEqual',
        'assertLessThanOrEqual' => 'assertGreaterThan',
        'assertArrayHasKey' => 'assertArrayNotHasKey',
        'assertFileExists' => 'assertFileDoesNotExist',
        'assertDirectoryExists' => 'assertDirectoryDoesNotExist',
        'assertJson' => 'assertIsNotString', // no direct negation, fallback
        'assertEqualsCanonicalizing' => 'assertNotEqualsCanonicalizing',
        'assertEqualsWithDelta' => 'assertNotEqualsWithDelta',
        'assertSameSize' => 'assertNotSameSize',
        'assertContainsEquals' => 'assertNotContainsEquals',
        'assertFileIsReadable' => 'assertFileIsNotReadable',
        'assertFileIsWritable' => 'assertFileIsNotWritable',
        'assertDirectoryIsReadable' => 'assertDirectoryIsNotReadable',
        'assertDirectoryIsWritable' => 'assertDirectoryIsNotWritable',
        'assertIsFinite' => 'assertInfinite',
        'assertIsInfinite' => 'assertFinite',
    ];

    /**
     * Known modifier property/method names.
     *
     * @var list<string>
     */
    public const MODIFIERS = [
        'not',
        'each',
        'and',
        'sequence',
        'json',
        'defer',
        'ray',
    ];

    public static function isTerminal(string $name): bool
    {
        return isset(self::MAP[$name]);
    }

    public static function isModifier(string $name): bool
    {
        return in_array($name, self::MODIFIERS, true);
    }

    /**
     * @return array{string, string}|null
     */
    public static function getMapping(string $pestMethod): ?array
    {
        return self::MAP[$pestMethod] ?? null;
    }

    public static function getNegated(string $phpunitMethod): ?string
    {
        return self::NEGATED_MAP[$phpunitMethod] ?? null;
    }
}
