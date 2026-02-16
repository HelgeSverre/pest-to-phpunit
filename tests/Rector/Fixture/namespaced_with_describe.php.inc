<?php

namespace Tests\Unit;

describe('StringHelper', function () {
    test('slugifies', function () {
        expect(slugify('Hello World'))->toBe('hello-world');
    });

    test('truncates', function () {
        expect(truncate('Long text', 4))->toBe('Long...');
    });
});
-----
<?php

namespace Tests\Unit;

class NamespacedWithDescribeTest extends \PHPUnit\Framework\TestCase
{
    public function test_stringhelper_slugifies(): void
    {
        $this->assertSame('hello-world', slugify('Hello World'));
    }
    public function test_stringhelper_truncates(): void
    {
        $this->assertSame('Long...', truncate('Long text', 4));
    }
}
