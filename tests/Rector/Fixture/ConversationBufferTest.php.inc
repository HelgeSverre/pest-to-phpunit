<?php

use HelgeSverre\Swarm\Agent\ConversationBuffer;

test('conversation buffer manages memory correctly', function () {
    $buffer = new ConversationBuffer(10); // Small buffer for testing

    // Add messages up to limit
    for ($i = 0; $i < 15; $i++) {
        $buffer->addMessage('user', "Test message {$i}");
    }

    $stats = $buffer->getStats();
    expect($stats['message_count'])->toBeLessThanOrEqual(10);
    expect($stats)->toHaveKey('max_size');
    expect($stats)->toHaveKey('memory_usage');
});

test('conversation buffer calculates relevance correctly', function () {
    $buffer = new ConversationBuffer;

    // Add messages with different relevance
    $buffer->addMessage('user', 'How do I create a PHP class?');
    $buffer->addMessage('assistant', 'You can create a PHP class using the class keyword...');
    $buffer->addMessage('user', 'What is the weather like?');
    $buffer->addMessage('user', 'Can you help me implement a PHP function?');

    // Get context for PHP-related task
    $context = $buffer->getOptimalContext('Create a new PHP method', 1000);

    expect($context)->toBeArray();
    expect(count($context))->toBeGreaterThan(0);
});

test('conversation buffer estimates memory usage', function () {
    $buffer = new ConversationBuffer;

    // Add some test messages
    for ($i = 0; $i < 5; $i++) {
        $buffer->addMessage('user', str_repeat('test message ', 50));
    }

    $stats = $buffer->getStats();
    expect($stats['memory_usage'])->toMatch('/\d+(\.\d+)?\s+(B|KB|MB|GB)/');
});

test('conversation buffer prevents unbounded growth', function () {
    $buffer = new ConversationBuffer(50);

    // Add many messages (more than max)
    for ($i = 0; $i < 100; $i++) {
        $buffer->addMessage('user', "Message {$i}");
    }

    $stats = $buffer->getStats();
    expect($stats['message_count'])->toBeLessThanOrEqual(50);
});

test('conversation buffer selects relevant context', function () {
    $buffer = new ConversationBuffer;

    // Add various messages
    $buffer->addMessage('user', 'I need help with PHP arrays');
    $buffer->addMessage('assistant', 'PHP arrays can be created using array() or []');
    $buffer->addMessage('user', 'How about JavaScript objects?');
    $buffer->addMessage('assistant', 'JavaScript objects use {} syntax');
    $buffer->addMessage('user', 'Back to PHP - how do I iterate arrays?');

    // Get context for PHP array task
    $context = $buffer->getOptimalContext('Show me PHP array iteration methods', 2000);

    // Should have selected PHP-related messages
    $contextText = implode(' ', array_column($context, 'content'));
    expect($contextText)->toContain('PHP');
    expect($contextText)->toContain('array');
});

test('conversation buffer handles empty context gracefully', function () {
    $buffer = new ConversationBuffer;

    // No messages added
    $context = $buffer->getOptimalContext('Any task', 1000);

    expect($context)->toBeArray();
    expect($context)->toBeEmpty();
});

test('conversation buffer recent context fallback works', function () {
    $buffer = new ConversationBuffer;

    // Add some messages
    for ($i = 0; $i < 10; $i++) {
        $buffer->addMessage('user', "Message {$i}");
    }

    $recentContext = $buffer->getRecentContext(5);

    expect($recentContext)->toBeArray();
    expect(count($recentContext))->toBe(5);

    // Should contain the most recent messages
    $lastMessage = $recentContext[4]['content'] ?? '';
    expect($lastMessage)->toBe('Message 9');
});

-----
<?php

use HelgeSverre\Swarm\Agent\ConversationBuffer;
class ConversationBufferTest extends \PHPUnit\Framework\TestCase
{
    public function test_conversation_buffer_manages_memory_correctly(): void
    {
        $buffer = new ConversationBuffer(10);
        // Small buffer for testing
        // Add messages up to limit
        for ($i = 0; $i < 15; $i++) {
            $buffer->addMessage('user', "Test message {$i}");
        }
        $stats = $buffer->getStats();
        $this->assertLessThanOrEqual(10, $stats['message_count']);
        $this->assertArrayHasKey('max_size', $stats);
        $this->assertArrayHasKey('memory_usage', $stats);
    }
    public function test_conversation_buffer_calculates_relevance_correctly(): void
    {
        $buffer = new ConversationBuffer;
        // Add messages with different relevance
        $buffer->addMessage('user', 'How do I create a PHP class?');
        $buffer->addMessage('assistant', 'You can create a PHP class using the class keyword...');
        $buffer->addMessage('user', 'What is the weather like?');
        $buffer->addMessage('user', 'Can you help me implement a PHP function?');
        // Get context for PHP-related task
        $context = $buffer->getOptimalContext('Create a new PHP method', 1000);
        $this->assertIsArray($context);
        $this->assertGreaterThan(0, count($context));
    }
    public function test_conversation_buffer_estimates_memory_usage(): void
    {
        $buffer = new ConversationBuffer;
        // Add some test messages
        for ($i = 0; $i < 5; $i++) {
            $buffer->addMessage('user', str_repeat('test message ', 50));
        }
        $stats = $buffer->getStats();
        $this->assertMatchesRegularExpression('/\d+(\.\d+)?\s+(B|KB|MB|GB)/', $stats['memory_usage']);
    }
    public function test_conversation_buffer_prevents_unbounded_growth(): void
    {
        $buffer = new ConversationBuffer(50);
        // Add many messages (more than max)
        for ($i = 0; $i < 100; $i++) {
            $buffer->addMessage('user', "Message {$i}");
        }
        $stats = $buffer->getStats();
        $this->assertLessThanOrEqual(50, $stats['message_count']);
    }
    public function test_conversation_buffer_selects_relevant_context(): void
    {
        $buffer = new ConversationBuffer;
        // Add various messages
        $buffer->addMessage('user', 'I need help with PHP arrays');
        $buffer->addMessage('assistant', 'PHP arrays can be created using array() or []');
        $buffer->addMessage('user', 'How about JavaScript objects?');
        $buffer->addMessage('assistant', 'JavaScript objects use {} syntax');
        $buffer->addMessage('user', 'Back to PHP - how do I iterate arrays?');
        // Get context for PHP array task
        $context = $buffer->getOptimalContext('Show me PHP array iteration methods', 2000);
        // Should have selected PHP-related messages
        $contextText = implode(' ', array_column($context, 'content'));
        $this->assertContains('PHP', $contextText);
        $this->assertContains('array', $contextText);
    }
    public function test_conversation_buffer_handles_empty_context_gracefully(): void
    {
        $buffer = new ConversationBuffer;
        // No messages added
        $context = $buffer->getOptimalContext('Any task', 1000);
        $this->assertIsArray($context);
        $this->assertEmpty($context);
    }
    public function test_conversation_buffer_recent_context_fallback_works(): void
    {
        $buffer = new ConversationBuffer;
        // Add some messages
        for ($i = 0; $i < 10; $i++) {
            $buffer->addMessage('user', "Message {$i}");
        }
        $recentContext = $buffer->getRecentContext(5);
        $this->assertIsArray($recentContext);
        $this->assertSame(5, count($recentContext));
        // Should contain the most recent messages
        $lastMessage = $recentContext[4]['content'] ?? '';
        $this->assertSame('Message 9', $lastMessage);
    }
}
