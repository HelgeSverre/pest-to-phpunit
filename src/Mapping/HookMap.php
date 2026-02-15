<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Mapping;

final class HookMap
{
    public const MAP = [
        'beforeAll' => ['setUpBeforeClass', true],
        'beforeEach' => ['setUp', false],
        'afterEach' => ['tearDown', false],
        'afterAll' => ['tearDownAfterClass', true],
    ];

    public static function isHook(string $name): bool
    {
        return isset(self::MAP[$name]);
    }

    /**
     * @return array{string, bool}|null Returns [phpunitMethodName, isStatic]
     */
    public static function getMapping(string $pestHook): ?array
    {
        return self::MAP[$pestHook] ?? null;
    }
}
