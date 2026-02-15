<?php

test('read file tool generates correct schema', function () {
    $tool = new HelgeSverre\Swarm\Tools\ReadFile;
    $schema = $tool->toOpenAISchema();

    expect($schema)->toBeArray()
        ->and($schema['type'])->toBe('function')
        ->and($schema['function']['name'])->toBe('read_file')
        ->and($schema['function']['description'])->toBe('Read contents of a file from the filesystem')
        ->and($schema['function']['parameters']['type'])->toBe('object')
        ->and($schema['function']['parameters']['properties'])->toHaveKey('path')
        ->and($schema['function']['parameters']['properties']['path']['type'])->toBe('string')
        ->and($schema['function']['parameters']['properties']['path']['description'])->toBe('Path to the file to read')
        ->and($schema['function']['parameters']['required'])->toBe(['path']);
});

test('write file tool generates correct schema', function () {
    $tool = new HelgeSverre\Swarm\Tools\WriteFile;
    $schema = $tool->toOpenAISchema();

    expect($schema)->toBeArray()
        ->and($schema['type'])->toBe('function')
        ->and($schema['function']['name'])->toBe('write_file')
        ->and($schema['function']['description'])->toBe('Write content to a file')
        ->and($schema['function']['parameters']['type'])->toBe('object')
        ->and($schema['function']['parameters']['properties'])->toHaveKeys(['path', 'content'])
        ->and($schema['function']['parameters']['properties']['path']['type'])->toBe('string')
        ->and($schema['function']['parameters']['properties']['content']['type'])->toBe('string')
        ->and($schema['function']['parameters']['required'])->toBe(['path', 'content']);
});

test('tool executor collects schemas from registered tools', function () {
    $executor = new HelgeSverre\Swarm\Core\ToolExecutor;

    $executor->register(new HelgeSverre\Swarm\Tools\ReadFile);
    $executor->register(new HelgeSverre\Swarm\Tools\WriteFile);

    $schemas = $executor->getToolSchemas();

    expect($schemas)->toBeArray()
        ->and($schemas)->toHaveCount(2)
        ->and($schemas[0]['function']['name'])->toBe('read_file')
        ->and($schemas[1]['function']['name'])->toBe('write_file');
});

test('read file tool executes correctly', function () {
    $tool = new HelgeSverre\Swarm\Tools\ReadFile;

    // Create a test file
    $testFile = '/tmp/test_file.txt';
    file_put_contents($testFile, "Test content\nLine 2");

    $result = $tool->execute(['path' => $testFile]);

    expect($result->isSuccess())->toBeTrue()
        ->and($result->getData()['content'])->toBe("Test content\nLine 2")
        ->and($result->getData()['lines'])->toBe(2);

    // Clean up
    unlink($testFile);
});

test('read file tool returns error for non-existent file', function () {
    $tool = new HelgeSverre\Swarm\Tools\ReadFile;

    $result = $tool->execute(['path' => '/tmp/non_existent_file.txt']);

    expect($result->isSuccess())->toBeFalse()
        ->and($result->getError())->toContain('File not found');
});

test('write file tool executes correctly', function () {
    $tool = new HelgeSverre\Swarm\Tools\WriteFile;

    $testFile = '/tmp/test_write_file.txt';
    $content = 'New content';

    $result = $tool->execute([
        'path' => $testFile,
        'content' => $content,
    ]);

    expect($result->isSuccess())->toBeTrue()
        ->and($result->getData()['bytes_written'])->toBe(mb_strlen($content))
        ->and(file_exists($testFile))->toBeTrue()
        ->and(file_get_contents($testFile))->toBe($content);

    // Clean up
    unlink($testFile);
});

-----
<?php

class ToolSchemaTest extends \PHPUnit\Framework\TestCase
{
    public function test_read_file_tool_generates_correct_schema(): void
    {
        $tool = new HelgeSverre\Swarm\Tools\ReadFile;
        $schema = $tool->toOpenAISchema();
        $this->assertIsArray($schema);
        $this->assertSame('function', $schema['type']);
        $this->assertSame('read_file', $schema['function']['name']);
        $this->assertSame('Read contents of a file from the filesystem', $schema['function']['description']);
        $this->assertSame('object', $schema['function']['parameters']['type']);
        $this->assertArrayHasKey('path', $schema['function']['parameters']['properties']);
        $this->assertSame('string', $schema['function']['parameters']['properties']['path']['type']);
        $this->assertSame('Path to the file to read', $schema['function']['parameters']['properties']['path']['description']);
        $this->assertSame(['path'], $schema['function']['parameters']['required']);
    }
    public function test_write_file_tool_generates_correct_schema(): void
    {
        $tool = new HelgeSverre\Swarm\Tools\WriteFile;
        $schema = $tool->toOpenAISchema();
        $this->assertIsArray($schema);
        $this->assertSame('function', $schema['type']);
        $this->assertSame('write_file', $schema['function']['name']);
        $this->assertSame('Write content to a file', $schema['function']['description']);
        $this->assertSame('object', $schema['function']['parameters']['type']);
        $this->assertArrayHasKey('path', $schema['function']['parameters']['properties']);
        $this->assertArrayHasKey('content', $schema['function']['parameters']['properties']);
        $this->assertSame('string', $schema['function']['parameters']['properties']['path']['type']);
        $this->assertSame('string', $schema['function']['parameters']['properties']['content']['type']);
        $this->assertSame(['path', 'content'], $schema['function']['parameters']['required']);
    }
    public function test_tool_executor_collects_schemas_from_registered_tools(): void
    {
        $executor = new HelgeSverre\Swarm\Core\ToolExecutor;
        $executor->register(new HelgeSverre\Swarm\Tools\ReadFile);
        $executor->register(new HelgeSverre\Swarm\Tools\WriteFile);
        $schemas = $executor->getToolSchemas();
        $this->assertIsArray($schemas);
        $this->assertCount(2, $schemas);
        $this->assertSame('read_file', $schemas[0]['function']['name']);
        $this->assertSame('write_file', $schemas[1]['function']['name']);
    }
    public function test_read_file_tool_executes_correctly(): void
    {
        $tool = new HelgeSverre\Swarm\Tools\ReadFile;
        // Create a test file
        $testFile = '/tmp/test_file.txt';
        file_put_contents($testFile, "Test content\nLine 2");
        $result = $tool->execute(['path' => $testFile]);
        $this->assertTrue($result->isSuccess());
        $this->assertSame("Test content\nLine 2", $result->getData()['content']);
        $this->assertSame(2, $result->getData()['lines']);
        // Clean up
        unlink($testFile);
    }
    public function test_read_file_tool_returns_error_for_nonexistent_file(): void
    {
        $tool = new HelgeSverre\Swarm\Tools\ReadFile;
        $result = $tool->execute(['path' => '/tmp/non_existent_file.txt']);
        $this->assertFalse($result->isSuccess());
        $this->assertContains('File not found', $result->getError());
    }
    public function test_write_file_tool_executes_correctly(): void
    {
        $tool = new HelgeSverre\Swarm\Tools\WriteFile;
        $testFile = '/tmp/test_write_file.txt';
        $content = 'New content';
        $result = $tool->execute([
            'path' => $testFile,
            'content' => $content,
        ]);
        $this->assertTrue($result->isSuccess());
        $this->assertSame(mb_strlen($content), $result->getData()['bytes_written']);
        $this->assertTrue(file_exists($testFile));
        $this->assertSame($content, file_get_contents($testFile));
        // Clean up
        unlink($testFile);
    }
}
