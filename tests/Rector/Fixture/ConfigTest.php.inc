<?php

test('SVG is not in default allowed image types', function () {
    $allowedImageTypes = config('larapress.media.allowed_types.image');

    expect($allowedImageTypes)->not->toContain('svg');
});

test('default allowed image types include common safe formats', function () {
    $allowedImageTypes = config('larapress.media.allowed_types.image');

    expect($allowedImageTypes)->toContain('jpg')
        ->and($allowedImageTypes)->toContain('jpeg')
        ->and($allowedImageTypes)->toContain('png')
        ->and($allowedImageTypes)->toContain('gif')
        ->and($allowedImageTypes)->toContain('webp');
});
-----
<?php

class ConfigTest extends \PHPUnit\Framework\TestCase
{
    public function test_svg_is_not_in_default_allowed_image_types(): void
    {
        $allowedImageTypes = config('larapress.media.allowed_types.image');
        if (is_string($__pest_contain_subject = $allowedImageTypes)) {
            $this->assertStringNotContainsString('svg', $__pest_contain_subject);
        } else {
            $this->assertNotContains('svg', $__pest_contain_subject);
        }
    }
    public function test_default_allowed_image_types_include_common_safe_formats(): void
    {
        $allowedImageTypes = config('larapress.media.allowed_types.image');
        if (is_string($__pest_contain_subject = $allowedImageTypes)) {
            $this->assertStringContainsString('jpg', $__pest_contain_subject);
        } else {
            $this->assertContains('jpg', $__pest_contain_subject);
        }
        if (is_string($__pest_contain_subject = $allowedImageTypes)) {
            $this->assertStringContainsString('jpeg', $__pest_contain_subject);
        } else {
            $this->assertContains('jpeg', $__pest_contain_subject);
        }
        if (is_string($__pest_contain_subject = $allowedImageTypes)) {
            $this->assertStringContainsString('png', $__pest_contain_subject);
        } else {
            $this->assertContains('png', $__pest_contain_subject);
        }
        if (is_string($__pest_contain_subject = $allowedImageTypes)) {
            $this->assertStringContainsString('gif', $__pest_contain_subject);
        } else {
            $this->assertContains('gif', $__pest_contain_subject);
        }
        if (is_string($__pest_contain_subject = $allowedImageTypes)) {
            $this->assertStringContainsString('webp', $__pest_contain_subject);
        } else {
            $this->assertContains('webp', $__pest_contain_subject);
        }
    }
}
