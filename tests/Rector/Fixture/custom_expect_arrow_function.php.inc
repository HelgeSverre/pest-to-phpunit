<?php
// Source: defstudio/pest-plugin-laravel-expectations Collections.php
expect()->extend('toBeCollection', fn () => $this->toBeInstanceOf(\Illuminate\Support\Collection::class));

test('is a collection', function () {
    expect($items)->toBeCollection();
});

-----
<?php

class CustomExpectArrowFunctionTest extends \PHPUnit\Framework\TestCase
{
    public function test_is_a_collection(): void
    {
        $this->assertInstanceOf(\Illuminate\Support\Collection::class, $items);
    }
}
