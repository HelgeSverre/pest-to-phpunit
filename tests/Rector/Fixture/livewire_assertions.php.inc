<?php
uses(Tests\TestCase::class);

test('livewire component renders', function () {
    $component = Livewire::test(Counter::class);
    expect($component)->assertSee('Count: 0');
});

test('livewire set and check', function () {
    $component = Livewire::test(Counter::class);
    expect($component)->assertSet('count', 0);
});

test('livewire dispatches event', function () {
    $component = Livewire::test(Counter::class)->call('increment');
    expect($component)->assertDispatched('incremented');
});

test('livewire has no errors', function () {
    $component = Livewire::test(FormComponent::class)->call('submit');
    expect($component)->assertHasNoErrors();
});

-----
<?php

class LivewireAssertionsTest extends \Tests\TestCase
{
    public function test_livewire_component_renders(): void
    {
        $component = Livewire::test(Counter::class);
        $component->assertSee('Count: 0');
    }
    public function test_livewire_set_and_check(): void
    {
        $component = Livewire::test(Counter::class);
        $component->assertSet('count', 0);
    }
    public function test_livewire_dispatches_event(): void
    {
        $component = Livewire::test(Counter::class)->call('increment');
        $component->assertDispatched('incremented');
    }
    public function test_livewire_has_no_errors(): void
    {
        $component = Livewire::test(FormComponent::class)->call('submit');
        $component->assertHasNoErrors();
    }
}
