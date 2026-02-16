<?php

use function Pest\Faker\fake;

test('generates a name', function () {
    $name = fake()->name;
    expect($name)->toBeString();
});

test('with locale', function () {
    $name = fake('pt_PT')->name;
    expect($name)->toBeString();
});

-----
<?php

class ReadmeFakerTest extends \PHPUnit\Framework\TestCase
{
    public function test_generates_a_name(): void
    {
        $name = \Faker\Factory::create()->name;
        $this->assertIsString($name);
    }
    public function test_with_locale(): void
    {
        $name = \Faker\Factory::create('pt_PT')->name;
        $this->assertIsString($name);
    }
}
