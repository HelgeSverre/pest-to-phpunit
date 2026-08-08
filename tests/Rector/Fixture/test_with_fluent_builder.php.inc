<?php
test('fluent builder', function () {
    $query = User::query()
        ->where('active', true)
        ->where('age', '>', 18)
        ->orderBy('name')
        ->limit(10);

    expect($query->toSql())->toContain('where');
});
-----
<?php
class TestWithFluentBuilderTest extends \PHPUnit\Framework\TestCase
{
    public function test_fluent_builder(): void
    {
        $query = User::query()
            ->where('active', true)
            ->where('age', '>', 18)
            ->orderBy('name')
            ->limit(10);
        if (is_string($__pest_contain_subject = $query->toSql())) {
            $this->assertStringContainsString('where', $__pest_contain_subject);
        } else {
            $this->assertContains('where', $__pest_contain_subject);
        }
    }
}
