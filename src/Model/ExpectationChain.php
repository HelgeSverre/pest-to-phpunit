<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Model;

use PhpParser\Node\Expr;

final class ExpectationChain
{
    /** @var list<ChainSegment> */
    public array $accessors = [];

    /** @var list<string> */
    public array $modifiers = [];

    public ?string $terminal = null;

    /** @var list<Expr> */
    public array $terminalArgs = [];

    public function __construct(
        public readonly ?Expr $subject,
        public readonly bool $isContextual = false,
    ) {}
}
