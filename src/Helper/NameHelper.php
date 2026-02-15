<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Helper;

final class NameHelper
{
    public static function descriptionToMethodName(string $description, string $prefix = 'it'): string
    {
        // Remove "it " prefix if using it() syntax
        if ($prefix === 'it' && ! str_starts_with(strtolower($description), 'it ')) {
            $description = 'it ' . $description;
        }

        // Convert to snake_case method name
        $name = preg_replace('/[^a-zA-Z0-9\s]/', '', $description);
        $name = preg_replace('/\s+/', '_', trim($name));
        $name = strtolower($name);

        return 'test_' . $name;
    }

    public static function fileNameToClassName(string $fileName): string
    {
        $name = pathinfo($fileName, PATHINFO_FILENAME);

        // Convert to PascalCase
        $name = str_replace(['-', '_', '.'], ' ', $name);
        $name = ucwords($name);
        $name = str_replace(' ', '', $name);

        // Ensure it ends with Test
        if (! str_ends_with($name, 'Test')) {
            $name .= 'Test';
        }

        return $name;
    }
}
