<?php
test('typed params no data', function () {
    $x = getValue();
    expect($x)->toBeInt();
});
-----
<?php
class TestTypedParamsNoDatasetTest extends \PHPUnit\Framework\TestCase
{
    public function test_typed_params_no_data(): void
    {
        $x = getValue();
        $this->assertIsInt($x);
    }
}
