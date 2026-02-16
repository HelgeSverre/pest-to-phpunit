<?php
uses(Tests\TestCase::class);

test('livewire chained setup and assert', function () {
    expect(Livewire::test(CreatePost::class)->set('title', 'My Post')->set('content', 'Body')->call('save'))->assertHasNoErrors()->assertSee('Post saved');
});

test('livewire dont see', function () {
    $component = Livewire::test(Counter::class);
    expect($component)->assertDontSee('Error');
});

test('livewire see html', function () {
    $component = Livewire::test(Counter::class);
    expect($component)->assertSeeHtml('<span>Count: 0</span>');
});

test('livewire not dispatched', function () {
    $component = Livewire::test(Counter::class);
    expect($component)->assertNotDispatched('reset');
});

test('livewire has errors', function () {
    $component = Livewire::test(FormComponent::class)->set('email', '')->call('submit');
    expect($component)->assertHasErrors(['email']);
});

test('livewire redirect', function () {
    $component = Livewire::test(FormComponent::class)->call('submit');
    expect($component)->assertRedirect('/success');
});

test('livewire no redirect', function () {
    $component = Livewire::test(FormComponent::class)->call('validate');
    expect($component)->assertNoRedirect();
});

test('livewire not set', function () {
    $component = Livewire::test(Counter::class);
    expect($component)->assertNotSet('error');
});

test('livewire file downloaded', function () {
    $component = Livewire::test(ExportComponent::class)->call('export');
    expect($component)->assertFileDownloaded('report.csv');
});

-----
<?php

class LivewireComprehensiveTest extends \Tests\TestCase
{
    public function test_livewire_chained_setup_and_assert(): void
    {
        Livewire::test(CreatePost::class)->set('title', 'My Post')->set('content', 'Body')->call('save')->assertHasNoErrors()->assertSee('Post saved');
    }
    public function test_livewire_dont_see(): void
    {
        $component = Livewire::test(Counter::class);
        $component->assertDontSee('Error');
    }
    public function test_livewire_see_html(): void
    {
        $component = Livewire::test(Counter::class);
        $component->assertSeeHtml('<span>Count: 0</span>');
    }
    public function test_livewire_not_dispatched(): void
    {
        $component = Livewire::test(Counter::class);
        $component->assertNotDispatched('reset');
    }
    public function test_livewire_has_errors(): void
    {
        $component = Livewire::test(FormComponent::class)->set('email', '')->call('submit');
        $component->assertHasErrors(['email']);
    }
    public function test_livewire_redirect(): void
    {
        $component = Livewire::test(FormComponent::class)->call('submit');
        $component->assertRedirect('/success');
    }
    public function test_livewire_no_redirect(): void
    {
        $component = Livewire::test(FormComponent::class)->call('validate');
        $component->assertNoRedirect();
    }
    public function test_livewire_not_set(): void
    {
        $component = Livewire::test(Counter::class);
        $component->assertNotSet('error');
    }
    public function test_livewire_file_downloaded(): void
    {
        $component = Livewire::test(ExportComponent::class)->call('export');
        $component->assertFileDownloaded('report.csv');
    }
}
