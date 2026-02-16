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

    /**
     * Infer a namespace from a file path using composer.json PSR-4 mappings.
     * Returns null if no matching mapping is found.
     */
    public static function inferNamespaceFromPath(string $filePath): ?string
    {
        // Normalize the file path
        $filePath = str_replace('\\', '/', realpath($filePath) ?: $filePath);
        $dir = dirname($filePath);

        // Walk up to find composer.json
        $composerDir = $dir;
        $composerJson = null;
        while ($composerDir !== '/' && $composerDir !== '') {
            $candidate = $composerDir . '/composer.json';
            if (file_exists($candidate)) {
                $composerJson = $candidate;
                break;
            }
            $composerDir = dirname($composerDir);
        }

        if ($composerJson === null) {
            return null;
        }

        $content = file_get_contents($composerJson);
        if ($content === false) {
            return null;
        }

        $data = json_decode($content, true);
        if (! is_array($data)) {
            return null;
        }

        $projectRoot = str_replace('\\', '/', dirname($composerJson));

        // Collect PSR-4 mappings from both autoload and autoload-dev
        $psr4Mappings = [];
        foreach (['autoload-dev', 'autoload'] as $section) {
            if (isset($data[$section]['psr-4']) && is_array($data[$section]['psr-4'])) {
                foreach ($data[$section]['psr-4'] as $prefix => $paths) {
                    $paths = is_array($paths) ? $paths : [$paths];
                    foreach ($paths as $path) {
                        $absPath = $projectRoot . '/' . rtrim($path, '/');
                        $psr4Mappings[] = [$prefix, $absPath];
                    }
                }
            }
        }

        // Sort by path length descending (most specific match first)
        usort($psr4Mappings, fn ($a, $b) => strlen($b[1]) - strlen($a[1]));

        foreach ($psr4Mappings as [$prefix, $basePath]) {
            if (str_starts_with($dir, $basePath)) {
                $relative = substr($dir, strlen($basePath));
                $relative = trim($relative, '/');
                $ns = rtrim($prefix, '\\');
                if ($relative !== '') {
                    $ns .= '\\' . str_replace('/', '\\', $relative);
                }
                return $ns;
            }
        }

        return null;
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
