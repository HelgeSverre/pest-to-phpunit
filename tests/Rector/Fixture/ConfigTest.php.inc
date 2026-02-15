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
        $this->assertNotContains('svg', $allowedImageTypes);
    }
    public function test_default_allowed_image_types_include_common_safe_formats(): void
    {
        $allowedImageTypes = config('larapress.media.allowed_types.image');
        $this->assertContains('jpg', $allowedImageTypes);
        $this->assertContains('jpeg', $allowedImageTypes);
        $this->assertContains('png', $allowedImageTypes);
        $this->assertContains('gif', $allowedImageTypes);
        $this->assertContains('webp', $allowedImageTypes);
    }
}
