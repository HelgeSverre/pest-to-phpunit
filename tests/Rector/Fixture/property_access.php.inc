<?php

test('property access', function () {
    expect($user)->name->toBe('John');
    expect($user)->email->toContain('@');
    expect($response)->status->toBe(200);
});
-----
<?php

class PropertyAccessTest extends \PHPUnit\Framework\TestCase
{
    public function test_property_access(): void
    {
        $this->assertSame('John', $user->name);
        if (is_string($__pest_contain_subject = $user->email)) {
            $this->assertStringContainsString('@', $__pest_contain_subject);
        } else {
            $this->assertContains('@', $__pest_contain_subject);
        }
        $this->assertSame(200, $response->status);
    }
}
