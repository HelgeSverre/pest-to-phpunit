<?php
uses(
    Tests\TestCase::class,
    Illuminate\Foundation\Testing\RefreshDatabase::class,
    Illuminate\Foundation\Testing\WithFaker::class,
);

test('with faker', function () {
    expect($this->faker->name())->toBeString();
});
-----
<?php

class UsesMultipleTraitsTest extends \Tests\TestCase
{
    use Illuminate\Foundation\Testing\RefreshDatabase, Illuminate\Foundation\Testing\WithFaker;
    public function test_with_faker(): void
    {
        $this->assertIsString($this->faker->name());
    }
}
