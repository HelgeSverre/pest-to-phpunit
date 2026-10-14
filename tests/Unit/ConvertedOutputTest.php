<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Tests\Unit;

use HelgeSverre\PestToPhpUnit\Helper\NameHelper;
use PHPUnit\Framework\TestCase;

final class ConvertedOutputTest extends TestCase
{
    public function test_converted_datasets_and_numeric_assertions_run_in_phpunit(): void
    {
        $directory = sys_get_temp_dir() . '/pest-to-phpunit-' . bin2hex(random_bytes(8));
        mkdir($directory);
        $fixtures = ['readme_datasets', 'dataset_mixed_rows', 'finite_infinite', 'dataset_callable_scalars', 'to_have_properties_dynamic_key', 'dataset_dynamic_rows'];

        try {
            foreach ($fixtures as $fixture) {
                file_put_contents($directory . '/' . NameHelper::fileNameToClassName($fixture . '.php') . '.php', self::fixtureOutput($fixture));
            }

            $root = dirname(__DIR__, 2);
            $process = proc_open([
                PHP_BINARY, $root . '/vendor/bin/phpunit', '--no-configuration',
                '--bootstrap', $root . '/vendor/autoload.php', '--colors=never', $directory,
            ], [1 => ['pipe', 'w'], 2 => ['pipe', 'w']], $pipes);
            $this->assertIsResource($process);
            $stdout = stream_get_contents($pipes[1]);
            $stderr = stream_get_contents($pipes[2]);
            fclose($pipes[1]);
            fclose($pipes[2]);
            $this->assertSame(0, proc_close($process), $stdout . $stderr);
            $this->assertStringContainsString('OK (19 tests,', $stdout);
        } finally {
            foreach (glob($directory . '/*') as $file) {
                unlink($file);
            }
            rmdir($directory);
        }
    }

    public function test_converted_laravel_response_assertions_run(): void
    {
        if (! class_exists(\Illuminate\Foundation\Testing\TestCase::class)) {
            $this->markTestSkipped('Laravel is installed in the Laravel/Pest compatibility jobs.');
        }

        if (! class_exists('LaravelResponseRuntimeTest', false)) {
            eval(substr(self::fixtureOutput('laravel_response_runtime'), strlen('<?php')));
        }
        $test = new \LaravelResponseRuntimeTest('test_response_assertions');
        $test->test_response_assertions();
        $this->assertInstanceOf(\Illuminate\Foundation\Testing\TestCase::class, $test);
    }

    private static function fixtureOutput(string $fixture): string
    {
        $content = file_get_contents(__DIR__ . '/../Rector/Fixture/' . $fixture . '.php.inc');
        return explode("-----\n", $content, 2)[1];
    }
}
