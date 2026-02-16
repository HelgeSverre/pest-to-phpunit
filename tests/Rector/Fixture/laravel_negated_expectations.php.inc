<?php
test('negated laravel', function () {
    expect($x)->not->toBeCollection();
    expect($x)->not->toBeModel();
    expect($x)->not->toBeEloquentCollection();
});
-----
<?php
class LaravelNegatedExpectationsTest extends \PHPUnit\Framework\TestCase
{
    public function test_negated_laravel(): void
    {
        $this->assertNotInstanceOf(\Illuminate\Support\Collection::class, $x);
        $this->assertNotInstanceOf(\Illuminate\Database\Eloquent\Model::class, $x);
        $this->assertNotInstanceOf(\Illuminate\Database\Eloquent\Collection::class, $x);
    }
}
