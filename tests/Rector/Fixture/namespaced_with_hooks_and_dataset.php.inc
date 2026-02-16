<?php

namespace Tests\Integration;

use App\Services\Cache;

beforeEach(function () {
    $this->cache = new Cache();
});

dataset('cache keys', ['user:1', 'post:42', 'config:app']);

test('caches value', function (string $key) {
    $this->cache->set($key, 'value');
    expect($this->cache->get($key))->toBe('value');
})->with('cache keys');
-----
<?php

namespace Tests\Integration;

use App\Services\Cache;
class NamespacedWithHooksAndDatasetTest extends \PHPUnit\Framework\TestCase
{
    protected $cache;
    public static function cache_keys(): array
    {
        return ['user:1', 'post:42', 'config:app'];
    }
    protected function setUp(): void
    {
        parent::setUp();
        $this->cache = new Cache();
    }
    #[\PHPUnit\Framework\Attributes\DataProvider('cache_keys')]
    public function test_caches_value(string $key): void
    {
        $this->cache->set($key, 'value');
        $this->assertSame('value', $this->cache->get($key));
    }
}
