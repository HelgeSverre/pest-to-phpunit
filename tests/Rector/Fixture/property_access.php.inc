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
        $this->assertContains('@', $user->email);
        $this->assertSame(200, $response->status);
    }
}
