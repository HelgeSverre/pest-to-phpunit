<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Model;

use PhpParser\Node\Arg;

final class ChainSegment
{
    /**
     * @param list<Arg> $args
     */
    public function __construct(
        public readonly string $name,
        public readonly SegmentType $type,
        public readonly array $args = [],
    ) {}
}
