<?php
uses(\Tests\Feature\FeatureTestCase::class);

test('feature', function () {
    expect(true)->toBeTrue();
});
-----
<?php

class UsesTestcaseFqnTest extends \Tests\Feature\FeatureTestCase
{
    public function test_feature(): void
    {
        $this->assertTrue(true);
    }
}
