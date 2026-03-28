<?php
describe('Architecture', function () {
    arch('strict types')
        ->expect('App')
        ->toUseStrictTypes();
});
-----
<?php
class ArchInDescribeTest extends \PHPUnit\Framework\TestCase
{
}
