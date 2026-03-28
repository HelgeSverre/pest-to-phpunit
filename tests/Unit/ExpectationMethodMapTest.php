<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Tests\Unit;

use HelgeSverre\PestToPhpUnit\Mapping\ExpectationMethodMap;
use PHPUnit\Framework\TestCase;

final class ExpectationMethodMapTest extends TestCase
{
    public function test_every_map_entry_has_valid_arg_order(): void
    {
        $validArgOrders = ['expected_actual', 'actual_only'];

        foreach (ExpectationMethodMap::MAP as $pestMethod => $mapping) {
            $this->assertCount(2, $mapping, "MAP entry '{$pestMethod}' should have exactly 2 elements.");
            $this->assertContains(
                $mapping[1],
                $validArgOrders,
                "MAP entry '{$pestMethod}' has invalid arg order '{$mapping[1]}'. Expected one of: " . implode(', ', $validArgOrders),
            );
        }
    }

    public function test_every_map_phpunit_method_has_negation(): void
    {
        // These PHPUnit methods intentionally lack negated counterparts
        $intentionallyExcluded = [
            'assertNan',
            'assertJson',
            'assertIsList',
        ];

        $missing = [];

        foreach (ExpectationMethodMap::MAP as $pestMethod => $mapping) {
            $phpunitMethod = $mapping[0];

            if (in_array($phpunitMethod, $intentionallyExcluded, true)) {
                continue;
            }

            if (ExpectationMethodMap::getNegated($phpunitMethod) === null) {
                $missing[] = "{$pestMethod} => {$phpunitMethod}";
            }
        }

        $this->assertSame(
            [],
            $missing,
            'The following MAP entries have no NEGATED_MAP entry and are not intentionally excluded: ' . implode(', ', $missing),
        );
    }

    public function test_negated_map_references_valid_methods(): void
    {
        // Some NEGATED_MAP entries exist for methods used outside the MAP constant
        // (e.g. toContain is handled specially but still needs negation support)
        $knownExtras = [
            'assertContains',
            'assertStringContainsString',
        ];

        $validPhpunitMethods = array_column(ExpectationMethodMap::MAP, 0);

        $unexpected = [];

        foreach (ExpectationMethodMap::NEGATED_MAP as $method => $negated) {
            if (in_array($method, $knownExtras, true)) {
                continue;
            }

            if (! in_array($method, $validPhpunitMethods, true)) {
                $unexpected[] = "{$method} => {$negated}";
            }
        }

        $this->assertSame(
            [],
            $unexpected,
            'The following NEGATED_MAP keys do not appear as PHPUnit methods in MAP and are not known extras: ' . implode(', ', $unexpected),
        );
    }

    public function test_is_terminal_returns_true_for_map_keys(): void
    {
        $this->assertTrue(ExpectationMethodMap::isTerminal('toBe'));
        $this->assertTrue(ExpectationMethodMap::isTerminal('toEqual'));
        $this->assertTrue(ExpectationMethodMap::isTerminal('toBeNull'));
        $this->assertTrue(ExpectationMethodMap::isTerminal('toHaveCount'));
    }

    public function test_is_terminal_returns_false_for_unknown(): void
    {
        $this->assertFalse(ExpectationMethodMap::isTerminal('unknownMethod'));
    }

    public function test_is_modifier_returns_true_for_known_modifiers(): void
    {
        $this->assertTrue(ExpectationMethodMap::isModifier('not'));
        $this->assertTrue(ExpectationMethodMap::isModifier('each'));
        $this->assertTrue(ExpectationMethodMap::isModifier('and'));
        $this->assertTrue(ExpectationMethodMap::isModifier('tap'));
    }

    public function test_is_modifier_returns_false_for_terminals(): void
    {
        $this->assertFalse(ExpectationMethodMap::isModifier('toBe'));
        $this->assertFalse(ExpectationMethodMap::isModifier('toEqual'));
    }

    public function test_get_mapping_returns_null_for_unknown(): void
    {
        $this->assertNull(ExpectationMethodMap::getMapping('unknownMethod'));
    }

    public function test_get_negated_returns_null_for_unmapped(): void
    {
        $this->assertNull(ExpectationMethodMap::getNegated('someRandomMethod'));
    }

    public function test_no_overlap_between_terminals_and_modifiers(): void
    {
        $overlap = array_intersect(
            array_keys(ExpectationMethodMap::MAP),
            ExpectationMethodMap::MODIFIERS,
        );

        $this->assertSame(
            [],
            $overlap,
            'The following names appear in both MAP and MODIFIERS: ' . implode(', ', $overlap),
        );
    }
}
