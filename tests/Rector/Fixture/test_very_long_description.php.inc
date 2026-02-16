<?php
test('this is a very long test description that contains many words and should still generate a valid method name without any issues whatsoever', function () {
    expect(true)->toBeTrue();
});
-----
<?php
class TestVeryLongDescriptionTest extends \PHPUnit\Framework\TestCase
{
    public function test_this_is_a_very_long_test_description_that_contains_many_words_and_should_still_generate_a_valid_method_name_without_any_issues_whatsoever(): void
    {
        $this->assertTrue(true);
    }
}
