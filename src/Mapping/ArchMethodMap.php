<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Mapping;

/**
 * Maps Pest arch assertion methods to conversion strategies.
 *
 * Strategy types:
 *   'targeted'       — iterate layer, check each ObjectDescription via reflection
 *   'dependency'     — use ArchitectureAsserts::assertDependOn / assertDoesNotDependOn
 *   'function_usage' — iterate layer, check $object->uses for function names
 *   'file_content'   — iterate layer, check file contents with regex/string matching
 *
 * Each entry: 'pestMethod' => [strategy, assertionKey, supportsNegation]
 */
final class ArchMethodMap
{
    /**
     * @var array<string, array{string, string, bool}>
     */
    public const MAP = [
        // Type assertions (targeted, reflection)
        'toBeClasses'           => ['targeted', 'isClass', true],
        'toBeClass'             => ['targeted', 'isClass', true],
        'toBeInterfaces'        => ['targeted', 'isInterface', true],
        'toBeInterface'         => ['targeted', 'isInterface', true],
        'toBeTraits'            => ['targeted', 'isTrait', true],
        'toBeTrait'             => ['targeted', 'isTrait', true],
        'toBeEnums'             => ['targeted', 'isEnum', true],
        'toBeEnum'              => ['targeted', 'isEnum', true],
        'toBeFinal'             => ['targeted', 'isFinal', true],
        'toBeAbstract'          => ['targeted', 'isAbstract', true],
        'toBeReadonly'          => ['targeted', 'isReadOnly', true],
        'toBeInvokable'         => ['targeted', 'hasInvoke', true],

        // Backed enum assertions
        'toBeIntBackedEnum'     => ['targeted', 'isIntBackedEnum', true],
        'toBeIntBackedEnums'    => ['targeted', 'isIntBackedEnum', true],
        'toBeStringBackedEnum'  => ['targeted', 'isStringBackedEnum', true],
        'toBeStringBackedEnums' => ['targeted', 'isStringBackedEnum', true],

        // Inheritance/interface/trait assertions (targeted, reflection with args)
        'toExtend'              => ['targeted', 'extendsClass', true],
        'toExtendNothing'       => ['targeted', 'extendsNothing', true],
        'toImplement'           => ['targeted', 'implementsInterface', true],
        'toImplementNothing'    => ['targeted', 'implementsNothing', true],
        'toOnlyImplement'       => ['targeted', 'onlyImplements', true],
        'toUseTrait'            => ['targeted', 'usesTrait', true],
        'toUseTraits'           => ['targeted', 'usesTrait', true],

        // Naming assertions (targeted, reflection)
        'toHavePrefix'          => ['targeted', 'hasPrefix', true],
        'toHaveSuffix'          => ['targeted', 'hasSuffix', true],

        // Method/constructor assertions (targeted, reflection)
        'toHaveMethod'          => ['targeted', 'hasMethod', true],
        'toHaveMethods'         => ['targeted', 'hasMethod', true],
        'toHaveConstructor'     => ['targeted', 'hasConstructor', true],
        'toHaveDestructor'      => ['targeted', 'hasDestructor', true],

        // Attribute assertions (targeted, reflection)
        'toHaveAttribute'       => ['targeted', 'hasAttribute', true],

        // Method visibility (only valid negated in Pest, but we support both for generation)
        'toHavePublicMethods'           => ['targeted', 'hasPublicMethods', true],
        'toHavePublicMethodsBesides'    => ['targeted', 'hasPublicMethodsBesides', true],
        'toHaveProtectedMethods'        => ['targeted', 'hasProtectedMethods', true],
        'toHaveProtectedMethodsBesides' => ['targeted', 'hasProtectedMethodsBesides', true],
        'toHavePrivateMethods'          => ['targeted', 'hasPrivateMethods', true],
        'toHavePrivateMethodsBesides'   => ['targeted', 'hasPrivateMethodsBesides', true],

        // File content assertions
        'toUseStrictTypes'      => ['file_content', 'strictTypes', true],
        'toUseStrictEquality'   => ['file_content', 'strictEquality', true],
        'toHaveMethodsDocumented'    => ['file_content', 'methodsDocumented', true],
        'toHavePropertiesDocumented' => ['file_content', 'propertiesDocumented', true],
        'toHaveLineCountLessThan'    => ['file_content', 'lineCountLessThan', false],

        // Dependency assertions
        'toUse'                 => ['dependency', 'dependOn', true],
        'toOnlyUse'             => ['dependency', 'onlyDependOn', false],
        'toUseNothing'          => ['dependency', 'useNothing', false],
        'toBeUsedIn'            => ['dependency', 'beUsedIn', true],
        'toOnlyBeUsedIn'        => ['dependency', 'onlyBeUsedIn', false],
        'toBeUsedInNothing'     => ['dependency', 'beUsedInNothing', false],

        // Special: only valid negated
        'toBeUsed'              => ['function_usage', 'beUsed', true],
    ];

    /**
     * Layer filter modifiers that narrow the layer before assertion.
     *
     * @var array<string, string>
     */
    public const FILTERS = [
        'classes'      => '_CLASS',
        'interfaces'   => '_INTERFACE',
        'traits'       => '_TRAIT',
        'enums'        => '_ENUM',
    ];

    /**
     * @return array{string, string, bool}|null  [strategy, assertionKey, supportsNegation]
     */
    public static function getMapping(string $pestMethod): ?array
    {
        return self::MAP[$pestMethod] ?? null;
    }

    public static function isFilter(string $name): bool
    {
        return isset(self::FILTERS[$name]);
    }

    public static function getFilterType(string $name): ?string
    {
        return self::FILTERS[$name] ?? null;
    }

    public static function isArchAssertion(string $name): bool
    {
        return isset(self::MAP[$name]);
    }
}
