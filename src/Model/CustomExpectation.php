<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Model;

use PhpParser\Node\Param;
use PhpParser\Node\Stmt;

final class CustomExpectation
{
    /**
     * @param list<Param> $params  The closure parameters (excluding implicit $this)
     * @param list<Stmt>  $body    The closure body statements
     * @param bool        $isArrow Whether this was an arrow function
     */
    public function __construct(
        public readonly string $name,
        public readonly array $params,
        public readonly array $body,
        public readonly bool $isArrow = false,
    ) {}
}
