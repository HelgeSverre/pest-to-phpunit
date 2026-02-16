<?php

use function Pest\Faker\fake;

test('fake helper generates a name', function () {
    $name = fake()->name;
    expect($name)->toBeString();
});

test('fake helper with locale', function () {
    $name = fake('pt_PT')->name;
    expect($name)->toBeString();
});

test('fake used in assertion', function () {
    $email = fake()->safeEmail();
    expect($email)->toContain('@');
});

test('fake assigned to variable and used', function () {
    $faker = fake();
    $number = $faker->randomNumber();
    expect($number)->toBeInt();
});

test('fake used inline in expect', function () {
    expect(fake()->boolean())->toBeBool();
});

test('fake with locale in expect', function () {
    expect(fake('en_US')->name)->toBeString();
});

-----
<?php

class FakerPluginTest extends \PHPUnit\Framework\TestCase
{
    public function test_fake_helper_generates_a_name(): void
    {
        $name = \Faker\Factory::create()->name;
        $this->assertIsString($name);
    }
    public function test_fake_helper_with_locale(): void
    {
        $name = \Faker\Factory::create('pt_PT')->name;
        $this->assertIsString($name);
    }
    public function test_fake_used_in_assertion(): void
    {
        $email = \Faker\Factory::create()->safeEmail();
        $this->assertContains('@', $email);
    }
    public function test_fake_assigned_to_variable_and_used(): void
    {
        $faker = \Faker\Factory::create();
        $number = $faker->randomNumber();
        $this->assertIsInt($number);
    }
    public function test_fake_used_inline_in_expect(): void
    {
        $this->assertIsBool(\Faker\Factory::create()->boolean());
    }
    public function test_fake_with_locale_in_expect(): void
    {
        $this->assertIsString(\Faker\Factory::create('en_US')->name);
    }
}
