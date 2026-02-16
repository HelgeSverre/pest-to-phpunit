<?php

describe('Config', function () {
    test('gets nested values', function () {
        $config = new Config;
        expect($config->get('book.title'))->toBeString();
        expect($config->get('book'))->toBeArray();
    });

    test('returns null for missing keys', function () {
        $config = new Config;
        expect($config->get('nonexistent'))->toBeNull();
    });

    test('streaming is boolean', function () {
        $streaming = getConfig('streaming');
        expect($streaming)->toBeIn([true, false]);
    });

    test('output path ends with slug', function () {
        $path = getOutputDir();
        expect($path)->toEndWith('my-slug');
    });

    test('all returns required keys', function () {
        $all = getAll();
        expect($all)->toHaveKeys(['book', 'api', 'output']);
    });

    test('throws for invalid profile', function () {
        expect(fn () => loadProfile('nonexistent'))
            ->toThrow(InvalidArgumentException::class, 'not found');
    });
})->skip('Requires config setup');

-----
<?php

class DescribeWithSkipAndThrowTest extends \PHPUnit\Framework\TestCase
{
    public function test_config_gets_nested_values(): void
    {
        $this->markTestSkipped('Requires config setup');
    }
    public function test_config_returns_null_for_missing_keys(): void
    {
        $this->markTestSkipped('Requires config setup');
    }
    public function test_config_streaming_is_boolean(): void
    {
        $this->markTestSkipped('Requires config setup');
    }
    public function test_config_output_path_ends_with_slug(): void
    {
        $this->markTestSkipped('Requires config setup');
    }
    public function test_config_all_returns_required_keys(): void
    {
        $this->markTestSkipped('Requires config setup');
    }
    public function test_config_throws_for_invalid_profile(): void
    {
        $this->markTestSkipped('Requires config setup');
    }
}
