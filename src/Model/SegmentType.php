<?php

declare(strict_types=1);

namespace HelgeSverre\PestToPhpUnit\Model;

enum SegmentType: string
{
    case Property = 'property';
    case MethodCall = 'method_call';
}
