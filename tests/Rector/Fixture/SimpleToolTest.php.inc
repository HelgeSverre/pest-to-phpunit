<?php

use Mindwave\Mindwave\Contracts\Tool;
use Mindwave\Mindwave\Tools\SimpleTool;

describe('SimpleTool', function () {
    describe('Constructor', function () {
        it('accepts name, description, and callback', function () {
            $tool = new SimpleTool(
                name: 'test_tool',
                description: 'A test tool',
                callback: fn ($input) => "Processed: $input"
            );

            expect($tool)->toBeInstanceOf(SimpleTool::class);
        });
    });

    describe('Tool Interface', function () {
        it('implements Tool interface', function () {
            $tool = new SimpleTool(
                name: 'test',
                description: 'test',
                callback: fn ($input) => $input
            );

            expect($tool)->toBeInstanceOf(Tool::class);
        });

        it('returns configured name', function () {
            $tool = new SimpleTool(
                name: 'custom_name',
                description: 'test',
                callback: fn ($input) => $input
            );

            expect($tool->name())->toBe('custom_name');
        });

        it('returns configured description', function () {
            $tool = new SimpleTool(
                name: 'test',
                description: 'A detailed description of the tool',
                callback: fn ($input) => $input
            );

            expect($tool->description())->toBe('A detailed description of the tool');
        });
    });

    describe('run()', function () {
        it('executes callback with input', function () {
            $tool = new SimpleTool(
                name: 'echo',
                description: 'Echoes input',
                callback: fn ($input) => "Echo: $input"
            );

            $result = $tool->run('Hello');

            expect($result)->toBe('Echo: Hello');
        });

        it('passes $this context to callback', function () {
            $capturedContext = null;

            $tool = new SimpleTool(
                name: 'context_test',
                description: 'Tests context',
                callback: function ($input) use (&$capturedContext) {
                    $capturedContext = $this;

                    return $input;
                }
            );

            $tool->run('test');

            expect($capturedContext)->toBeInstanceOf(SimpleTool::class);
        });

        it('returns string result from callback', function () {
            $tool = new SimpleTool(
                name: 'uppercase',
                description: 'Makes uppercase',
                callback: fn ($input) => strtoupper($input)
            );

            $result = $tool->run('hello');

            expect($result)->toBe('HELLO');
        });

        it('handles complex input processing', function () {
            $tool = new SimpleTool(
                name: 'json_tool',
                description: 'Processes JSON',
                callback: function ($input) {
                    $data = json_decode($input, true);

                    return json_encode(['processed' => true, 'original' => $data]);
                }
            );

            $result = $tool->run('{"key": "value"}');
            $decoded = json_decode($result, true);

            expect($decoded['processed'])->toBeTrue();
            expect($decoded['original']['key'])->toBe('value');
        });

        it('handles empty input', function () {
            $tool = new SimpleTool(
                name: 'test',
                description: 'test',
                callback: fn ($input) => "Input was: '$input'"
            );

            $result = $tool->run('');

            expect($result)->toBe("Input was: ''");
        });
    });
});

-----
<?php

use Mindwave\Mindwave\Contracts\Tool;
use Mindwave\Mindwave\Tools\SimpleTool;

class SimpleToolTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_simpletool_constructor_accepts_name_description_and_callback(): void
    {
        $tool = new SimpleTool(
            name: 'test_tool',
            description: 'A test tool',
            callback: fn ($input) => "Processed: $input"
        );
        $this->assertInstanceOf(SimpleTool::class, $tool);
    }
    public function test_it_simpletool_tool_interface_implements_tool_interface(): void
    {
        $tool = new SimpleTool(
            name: 'test',
            description: 'test',
            callback: fn ($input) => $input
        );
        $this->assertInstanceOf(Tool::class, $tool);
    }
    public function test_it_simpletool_tool_interface_returns_configured_name(): void
    {
        $tool = new SimpleTool(
            name: 'custom_name',
            description: 'test',
            callback: fn ($input) => $input
        );
        $this->assertSame('custom_name', $tool->name());
    }
    public function test_it_simpletool_tool_interface_returns_configured_description(): void
    {
        $tool = new SimpleTool(
            name: 'test',
            description: 'A detailed description of the tool',
            callback: fn ($input) => $input
        );
        $this->assertSame('A detailed description of the tool', $tool->description());
    }
    public function test_it_simpletool_run_executes_callback_with_input(): void
    {
        $tool = new SimpleTool(
            name: 'echo',
            description: 'Echoes input',
            callback: fn ($input) => "Echo: $input"
        );
        $result = $tool->run('Hello');
        $this->assertSame('Echo: Hello', $result);
    }
    public function test_it_simpletool_run_passes_this_context_to_callback(): void
    {
        $capturedContext = null;
        $tool = new SimpleTool(
            name: 'context_test',
            description: 'Tests context',
            callback: function ($input) use (&$capturedContext) {
                $capturedContext = $this;

                return $input;
            }
        );
        $tool->run('test');
        $this->assertInstanceOf(SimpleTool::class, $capturedContext);
    }
    public function test_it_simpletool_run_returns_string_result_from_callback(): void
    {
        $tool = new SimpleTool(
            name: 'uppercase',
            description: 'Makes uppercase',
            callback: fn ($input) => strtoupper($input)
        );
        $result = $tool->run('hello');
        $this->assertSame('HELLO', $result);
    }
    public function test_it_simpletool_run_handles_complex_input_processing(): void
    {
        $tool = new SimpleTool(
            name: 'json_tool',
            description: 'Processes JSON',
            callback: function ($input) {
                $data = json_decode($input, true);

                return json_encode(['processed' => true, 'original' => $data]);
            }
        );
        $result = $tool->run('{"key": "value"}');
        $decoded = json_decode($result, true);
        $this->assertTrue($decoded['processed']);
        $this->assertSame('value', $decoded['original']['key']);
    }
    public function test_it_simpletool_run_handles_empty_input(): void
    {
        $tool = new SimpleTool(
            name: 'test',
            description: 'test',
            callback: fn ($input) => "Input was: '$input'"
        );
        $result = $tool->run('');
        $this->assertSame("Input was: ''", $result);
    }
}
