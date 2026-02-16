<?php
uses(Tests\Feature\AbstractApiTestCase::class);

test('api test', function () {
    expect(true)->toBeTrue();
});
-----
<?php

class UsesAbstractTestCaseTest extends \Tests\Feature\AbstractApiTestCase
{
    public function test_api_test(): void
    {
        $this->assertTrue(true);
    }
}
