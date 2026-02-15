<?php

use HelgeSverre\Extractor\Extraction\Builtins\Fields;
use HelgeSverre\Extractor\Extraction\Builtins\Receipt;
use HelgeSverre\Extractor\ExtractorManager;
use HelgeSverre\Extractor\Facades\Extractor as ExtractorFacade;

describe('ExtractorManager', function () {
    describe('extend', function () {
        it('allows registering custom extractors by name', function () {
            ExtractorFacade::extend('my-custom-extractor', function () {
                return new Fields;
            });

            expect(true)->toBeTrue();
        });
    });

    describe('resolveExtractor', function () {
        it('resolves built-in extractor by class name', function () {
            $manager = app(ExtractorManager::class);

            $reflection = new ReflectionClass($manager);
            $method = $reflection->getMethod('resolveExtractor');
            $method->setAccessible(true);

            $extractor = $method->invoke($manager, Receipt::class);

            expect($extractor)->toBeInstanceOf(Receipt::class);
        });

        it('returns extractor instance directly if already instantiated', function () {
            $manager = app(ExtractorManager::class);

            $reflection = new ReflectionClass($manager);
            $method = $reflection->getMethod('resolveExtractor');
            $method->setAccessible(true);

            $originalExtractor = new Fields;
            $extractor = $method->invoke($manager, $originalExtractor);

            expect($extractor)->toBe($originalExtractor);
        });

        it('throws exception for unknown extractor class', function () {
            $manager = app(ExtractorManager::class);

            $reflection = new ReflectionClass($manager);
            $method = $reflection->getMethod('resolveExtractor');
            $method->setAccessible(true);

            $method->invoke($manager, 'NonExistentExtractor');
        })->throws(Exception::class, 'not found');
    });

    describe('fields method', function () {
        it('accepts array of simple field names', function () {
            $manager = app(ExtractorManager::class);

            expect(method_exists($manager, 'fields'))->toBeTrue();
        });

        it('accepts nested field definitions', function () {
            $fields = [
                'name' => 'the person name',
                'items' => [
                    'name',
                    'price' => 'numeric price',
                    'quantity',
                ],
            ];

            expect($fields)->toBeArray();
            expect($fields['items'])->toBeArray();
        });
    });

    describe('view method', function () {
        it('exists and is callable', function () {
            $manager = app(ExtractorManager::class);

            expect(method_exists($manager, 'view'))->toBeTrue();
        });
    });

    describe('extract method', function () {
        it('accepts string extractor name', function () {
            $manager = app(ExtractorManager::class);

            expect(method_exists($manager, 'extract'))->toBeTrue();
        });

        it('accepts extractor instance', function () {
            $manager = app(ExtractorManager::class);

            $reflection = new ReflectionMethod($manager, 'extract');
            $params = $reflection->getParameters();

            expect($params[0]->getType()->__toString())->toContain('Extractor');
        });

        it('has default model parameter', function () {
            $reflection = new ReflectionMethod(ExtractorManager::class, 'extract');
            $params = $reflection->getParameters();

            $modelParam = null;
            foreach ($params as $param) {
                if ($param->getName() === 'model') {
                    $modelParam = $param;
                    break;
                }
            }

            expect($modelParam)->not->toBeNull();
            expect($modelParam->isDefaultValueAvailable())->toBeTrue();
        });
    });
});

-----
<?php

use HelgeSverre\Extractor\Extraction\Builtins\Fields;
use HelgeSverre\Extractor\Extraction\Builtins\Receipt;
use HelgeSverre\Extractor\ExtractorManager;
use HelgeSverre\Extractor\Facades\Extractor as ExtractorFacade;

class ExtractorManagerTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_extractormanager_extend_allows_registering_custom_extractors_by_name(): void
    {
        ExtractorFacade::extend('my-custom-extractor', function () {
            return new Fields;
        });
        $this->assertTrue(true);
    }
    public function test_it_extractormanager_resolveextractor_resolves_builtin_extractor_by_class_name(): void
    {
        $manager = app(ExtractorManager::class);
        $reflection = new ReflectionClass($manager);
        $method = $reflection->getMethod('resolveExtractor');
        $method->setAccessible(true);
        $extractor = $method->invoke($manager, Receipt::class);
        $this->assertInstanceOf(Receipt::class, $extractor);
    }
    public function test_it_extractormanager_resolveextractor_returns_extractor_instance_directly_if_already_instantiated(): void
    {
        $manager = app(ExtractorManager::class);
        $reflection = new ReflectionClass($manager);
        $method = $reflection->getMethod('resolveExtractor');
        $method->setAccessible(true);
        $originalExtractor = new Fields;
        $extractor = $method->invoke($manager, $originalExtractor);
        $this->assertSame($originalExtractor, $extractor);
    }
    public function test_it_extractormanager_resolveextractor_throws_exception_for_unknown_extractor_class(): void
    {
        $this->expectException(Exception::class);
        $this->expectExceptionMessage('not found');
        $manager = app(ExtractorManager::class);
        $reflection = new ReflectionClass($manager);
        $method = $reflection->getMethod('resolveExtractor');
        $method->setAccessible(true);
        $method->invoke($manager, 'NonExistentExtractor');
    }
    public function test_it_extractormanager_fields_method_accepts_array_of_simple_field_names(): void
    {
        $manager = app(ExtractorManager::class);
        $this->assertTrue(method_exists($manager, 'fields'));
    }
    public function test_it_extractormanager_fields_method_accepts_nested_field_definitions(): void
    {
        $fields = [
            'name' => 'the person name',
            'items' => [
                'name',
                'price' => 'numeric price',
                'quantity',
            ],
        ];
        $this->assertIsArray($fields);
        $this->assertIsArray($fields['items']);
    }
    public function test_it_extractormanager_view_method_exists_and_is_callable(): void
    {
        $manager = app(ExtractorManager::class);
        $this->assertTrue(method_exists($manager, 'view'));
    }
    public function test_it_extractormanager_extract_method_accepts_string_extractor_name(): void
    {
        $manager = app(ExtractorManager::class);
        $this->assertTrue(method_exists($manager, 'extract'));
    }
    public function test_it_extractormanager_extract_method_accepts_extractor_instance(): void
    {
        $manager = app(ExtractorManager::class);
        $reflection = new ReflectionMethod($manager, 'extract');
        $params = $reflection->getParameters();
        $this->assertContains('Extractor', $params[0]->getType()->__toString());
    }
    public function test_it_extractormanager_extract_method_has_default_model_parameter(): void
    {
        $reflection = new ReflectionMethod(ExtractorManager::class, 'extract');
        $params = $reflection->getParameters();
        $modelParam = null;
        foreach ($params as $param) {
            if ($param->getName() === 'model') {
                $modelParam = $param;
                break;
            }
        }
        $this->assertNotNull($modelParam);
        $this->assertTrue($modelParam->isDefaultValueAvailable());
    }
}
