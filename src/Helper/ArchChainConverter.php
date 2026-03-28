<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Helper;

use HelgeSverre\PestToPhpUnit\Mapping\ArchMethodMap;
use PhpParser\Node\Arg;
use PhpParser\Node\Expr\Array_;
use PhpParser\Node\Scalar\String_;
use PhpParser\Node\Stmt;

/**
 * Converts a Pest arch() chain (already unwrapped into modifiers) into
 * PHPUnit-compatible AST statement nodes.
 *
 * Returns null when the chain cannot be converted, signalling the caller
 * to fall back to markTestSkipped.
 */
final class ArchChainConverter
{
    /**
     * @param list<array{name: string, args: list<Arg>}> $chainModifiers
     * @return list<Stmt>|null  null = cannot convert, fall back to markTestSkipped
     */
    public static function convert(array $chainModifiers): ?array
    {
        // ── 1. Parse the chain ────────────────────────────────────────────
        $expectNamespace = null;
        $expectFunctions = null;
        $negated = false;
        $typeFilters = [];
        $ignoringNamespaces = [];
        $terminalMethod = null;
        $terminalArgs = [];

        foreach ($chainModifiers as $modifier) {
            $name = $modifier['name'];
            $args = $modifier['args'];

            if ($name === 'expect') {
                if (count($args) >= 1) {
                    $value = $args[0]->value;
                    if ($value instanceof Array_) {
                        // expect(['dd', 'dump']) → function list mode
                        $functions = [];
                        foreach ($value->items as $item) {
                            if ($item !== null && $item->value instanceof String_) {
                                $functions[] = $item->value->value;
                            }
                        }
                        $expectFunctions = $functions;
                    } elseif ($value instanceof String_) {
                        $expectNamespace = $value->value;
                    } else {
                        // Dynamic expression — cannot convert
                        return null;
                    }
                }
                continue;
            }

            if ($name === 'not') {
                $negated = !$negated;
                continue;
            }

            if ($name === 'each') {
                // 'each' in arch context means "for each item in array, run assertion"
                // This is already handled by function_usage strategy iterating the array
                continue;
            }

            if ($name === 'ignoring') {
                foreach ($args as $arg) {
                    if ($arg->value instanceof String_) {
                        $ignoringNamespaces[] = $arg->value->value;
                    } elseif ($arg->value instanceof Array_) {
                        foreach ($arg->value->items as $item) {
                            if ($item !== null && $item->value instanceof String_) {
                                $ignoringNamespaces[] = $item->value->value;
                            }
                        }
                    }
                }
                continue;
            }

            if (ArchMethodMap::isFilter($name)) {
                $typeFilters[] = $name;
                continue;
            }

            if (ArchMethodMap::isArchAssertion($name)) {
                $terminalMethod = $name;
                $terminalArgs = $args;
                continue;
            }

            // Unknown modifier — bail out
            return null;
        }

        // Must have either a namespace or function list, and a terminal assertion
        if (($expectNamespace === null && $expectFunctions === null) || $terminalMethod === null) {
            return null;
        }

        // ── 2. Look up the strategy ──────────────────────────────────────
        $mapping = ArchMethodMap::getMapping($terminalMethod);
        if ($mapping === null) {
            return null;
        }

        [$strategy, $assertionKey, $supportsNegation] = $mapping;

        // If negation is used on an assertion that doesn't support it, bail out
        if ($negated && !$supportsNegation) {
            return null;
        }

        // ── 3. Delegate to the builder ───────────────────────────────────
        return match ($strategy) {
            'targeted' => ArchAssertionBuilder::buildTargeted(
                $expectNamespace,
                $assertionKey,
                $negated,
                $terminalArgs,
                $typeFilters,
                $ignoringNamespaces,
            ),
            'dependency' => ArchAssertionBuilder::buildDependency(
                $expectNamespace,
                $assertionKey,
                $negated,
                $terminalArgs,
                $typeFilters,
                $ignoringNamespaces,
            ),
            'function_usage' => ArchAssertionBuilder::buildFunctionUsage(
                $expectNamespace,
                $expectFunctions,
                $assertionKey,
                $negated,
                $typeFilters,
                $ignoringNamespaces,
            ),
            'file_content' => ArchAssertionBuilder::buildFileContent(
                $expectNamespace,
                $assertionKey,
                $negated,
                $terminalArgs,
                $typeFilters,
                $ignoringNamespaces,
            ),
            default => null,
        };
    }
}
