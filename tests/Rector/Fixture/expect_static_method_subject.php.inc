<?php
test('static method subject', function () {
    expect(Carbon::now()->year)->toBeInt();
});

-----
<?php
class ExpectStaticMethodSubjectTest extends \PHPUnit\Framework\TestCase
{
    public function test_static_method_subject(): void
    {
        $this->assertIsInt(Carbon::now()->year);
    }
}
