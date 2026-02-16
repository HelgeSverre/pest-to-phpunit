<?php

use function Pest\Faker\fake;

beforeEach(function () {
    $this->email = fake()->safeEmail();
});

test('multiple fake calls', function () {
    $name = fake()->name;
    $city = fake('de_DE')->city;
    expect($name)->toBeString()
        ->and($city)->toBeString();
});

test('fake in conditional', function () {
    $number = fake()->numberBetween(1, 100);
    if ($number > 50) {
        expect($number)->toBeGreaterThan(50);
    }
});

test('fake with chained method calls', function () {
    $sentence = fake()->unique()->sentence();
    expect($sentence)->toBeString();
});

-----
<?php

class FakerAdvancedTest extends \PHPUnit\Framework\TestCase
{
    protected $email;
    protected function setUp(): void
    {
        parent::setUp();
        $this->email = \Faker\Factory::create()->safeEmail();
    }
    public function test_multiple_fake_calls(): void
    {
        $name = \Faker\Factory::create()->name;
        $city = \Faker\Factory::create('de_DE')->city;
        $this->assertIsString($name);
        $this->assertIsString($city);
    }
    public function test_fake_in_conditional(): void
    {
        $number = \Faker\Factory::create()->numberBetween(1, 100);
        if ($number > 50) {
            $this->assertGreaterThan(50, $number);
        }
    }
    public function test_fake_with_chained_method_calls(): void
    {
        $sentence = \Faker\Factory::create()->unique()->sentence();
        $this->assertIsString($sentence);
    }
}
