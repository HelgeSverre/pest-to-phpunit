<?php

test('returns collection', function () {
    expect($result)->toBeCollection();
});

test('returns model', function () {
    expect($user)->toBeModel();
});

test('not a collection', function () {
    expect($result)->not->toBeCollection();
});

-----
<?php

class LaravelExpectationsTest extends \PHPUnit\Framework\TestCase
{
    public function test_returns_collection(): void
    {
        $this->assertInstanceOf(\Illuminate\Support\Collection::class, $result);
    }
    public function test_returns_model(): void
    {
        $this->assertInstanceOf(\Illuminate\Database\Eloquent\Model::class, $user);
    }
    public function test_not_a_collection(): void
    {
        $this->assertNotInstanceOf(\Illuminate\Support\Collection::class, $result);
    }
}
