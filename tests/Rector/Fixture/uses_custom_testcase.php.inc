<?php
uses(App\Testing\IntegrationTestCase::class);

test('integration', function () {
    expect($this->app)->not->toBeNull();
});
-----
<?php

class UsesCustomTestcaseTest extends \App\Testing\IntegrationTestCase
{
    public function test_integration(): void
    {
        $this->assertNotNull($this->app);
    }
}
