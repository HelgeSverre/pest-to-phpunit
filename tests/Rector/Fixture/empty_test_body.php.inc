<?php
test('empty body', function () {
});
-----
<?php
class EmptyTestBodyTest extends \PHPUnit\Framework\TestCase
{
    public function test_empty_body(): void
    {
    }
}
