# Agent Instructions

## Releases

When creating a release, always do **both**:

1. `git tag vX.Y.Z` — creates the git tag
2. `gh release create vX.Y.Z --title "vX.Y.Z" --notes "..."` — creates the GitHub release

A git tag alone does **not** show up as a release on GitHub. You need the `gh release create` step for it to appear in the Releases page and for Packagist to pick it up properly.

## Testing

Run the full test suite before any release:

```bash
vendor/bin/phpunit tests/Rector/PestFileToPhpUnitClassRectorTest.php
```

All fixtures in `tests/Rector/Fixture/` are automatically discovered and run. Fixtures prefixed with `readme_` verify the README code examples — if you change a README example, update the corresponding fixture (and vice versa).

## Fixture Format

Test fixtures use Rector's split format:

```
<?php
// Pest input (top half)

-----
<?php
// Expected PHPUnit output (bottom half)
```

The class name is derived from the fixture filename: `my_feature.php.inc` → `MyFeatureTest`.
