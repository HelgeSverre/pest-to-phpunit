<?php

declare(strict_types=1);

use Rector\Config\RectorConfig;
use HelgeSverre\PestToPhpUnit\Set\PestToPhpUnitSetList;

return RectorConfig::configure()
    ->withSets([
        PestToPhpUnitSetList::PEST_TO_PHPUNIT,
    ]);
