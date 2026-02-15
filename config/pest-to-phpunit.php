<?php

declare(strict_types=1);

use HelgeSverre\PestToPhpUnit\Rector\PestFileToPhpUnitClassRector;
use Rector\Config\RectorConfig;

return RectorConfig::configure()
    ->withRules([
        PestFileToPhpUnitClassRector::class,
    ]);
