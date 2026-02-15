<?php

declare(strict_types=1);

use HelgeSverre\Prunekeeper\Support\ArchiveResult;
use HelgeSverre\Prunekeeper\Tests\Fixtures\TestPrunableModel;

it('creates archive result with all properties', function () {
    $result = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'exports/test.csv',
        recordCount: 100,
        fileSize: 2048,
        format: 'csv',
        compressed: false
    );

    expect($result->modelClass)->toBe(TestPrunableModel::class);
    expect($result->storagePath)->toBe('exports/test.csv');
    expect($result->recordCount)->toBe(100);
    expect($result->fileSize)->toBe(2048);
    expect($result->format)->toBe('csv');
    expect($result->compressed)->toBeFalse();
});

it('converts to array correctly', function () {
    $result = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'exports/test.csv',
        recordCount: 100,
        fileSize: 2048,
        format: 'csv',
        compressed: true
    );

    $array = $result->toArray();

    expect($array)->toBe([
        'model_class' => TestPrunableModel::class,
        'storage_path' => 'exports/test.csv',
        'record_count' => 100,
        'file_size' => 2048,
        'format' => 'csv',
        'compressed' => true,
    ]);
});

it('formats file size in bytes', function () {
    $result = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'test.csv',
        recordCount: 1,
        fileSize: 500,
        format: 'csv',
        compressed: false
    );

    expect($result->humanFileSize())->toBe('500 B');
});

it('formats file size in kilobytes', function () {
    $result = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'test.csv',
        recordCount: 1,
        fileSize: 2048,
        format: 'csv',
        compressed: false
    );

    expect($result->humanFileSize())->toBe('2 KB');
});

it('formats file size in megabytes', function () {
    $result = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'test.csv',
        recordCount: 1,
        fileSize: 5 * 1024 * 1024, // 5 MB
        format: 'csv',
        compressed: false
    );

    expect($result->humanFileSize())->toBe('5 MB');
});

it('formats file size in gigabytes', function () {
    $result = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'test.csv',
        recordCount: 1,
        fileSize: 2 * 1024 * 1024 * 1024, // 2 GB
        format: 'csv',
        compressed: false
    );

    expect($result->humanFileSize())->toBe('2 GB');
});

it('formats file size in terabytes', function () {
    $result = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'test.csv',
        recordCount: 1,
        fileSize: 3 * 1024 * 1024 * 1024 * 1024, // 3 TB
        format: 'csv',
        compressed: false
    );

    expect($result->humanFileSize())->toBe('3 TB');
});

it('formats file size with decimal precision', function () {
    $result = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'test.csv',
        recordCount: 1,
        fileSize: 1536, // 1.5 KB
        format: 'csv',
        compressed: false
    );

    expect($result->humanFileSize())->toBe('1.5 KB');
});

it('handles zero file size', function () {
    $result = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'test.csv',
        recordCount: 0,
        fileSize: 0,
        format: 'csv',
        compressed: false
    );

    expect($result->humanFileSize())->toBe('0 B');
});

it('stores compressed flag correctly', function () {
    $compressedResult = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'test.csv.zip',
        recordCount: 1,
        fileSize: 100,
        format: 'csv',
        compressed: true
    );

    $uncompressedResult = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'test.csv',
        recordCount: 1,
        fileSize: 100,
        format: 'csv',
        compressed: false
    );

    expect($compressedResult->compressed)->toBeTrue();
    expect($uncompressedResult->compressed)->toBeFalse();
});

it('stores sql format correctly', function () {
    $result = new ArchiveResult(
        modelClass: TestPrunableModel::class,
        storagePath: 'test.sql',
        recordCount: 1,
        fileSize: 100,
        format: 'sql',
        compressed: false
    );

    expect($result->format)->toBe('sql');
});

-----
<?php

declare(strict_types=1);

use HelgeSverre\Prunekeeper\Support\ArchiveResult;
use HelgeSverre\Prunekeeper\Tests\Fixtures\TestPrunableModel;
class ArchiveResultTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_creates_archive_result_with_all_properties(): void
    {
        $result = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'exports/test.csv',
            recordCount: 100,
            fileSize: 2048,
            format: 'csv',
            compressed: false
        );
        $this->assertSame(TestPrunableModel::class, $result->modelClass);
        $this->assertSame('exports/test.csv', $result->storagePath);
        $this->assertSame(100, $result->recordCount);
        $this->assertSame(2048, $result->fileSize);
        $this->assertSame('csv', $result->format);
        $this->assertFalse($result->compressed);
    }
    public function test_it_converts_to_array_correctly(): void
    {
        $result = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'exports/test.csv',
            recordCount: 100,
            fileSize: 2048,
            format: 'csv',
            compressed: true
        );
        $array = $result->toArray();
        $this->assertSame([
            'model_class' => TestPrunableModel::class,
            'storage_path' => 'exports/test.csv',
            'record_count' => 100,
            'file_size' => 2048,
            'format' => 'csv',
            'compressed' => true,
        ], $array);
    }
    public function test_it_formats_file_size_in_bytes(): void
    {
        $result = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'test.csv',
            recordCount: 1,
            fileSize: 500,
            format: 'csv',
            compressed: false
        );
        $this->assertSame('500 B', $result->humanFileSize());
    }
    public function test_it_formats_file_size_in_kilobytes(): void
    {
        $result = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'test.csv',
            recordCount: 1,
            fileSize: 2048,
            format: 'csv',
            compressed: false
        );
        $this->assertSame('2 KB', $result->humanFileSize());
    }
    public function test_it_formats_file_size_in_megabytes(): void
    {
        $result = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'test.csv',
            recordCount: 1,
            fileSize: 5 * 1024 * 1024, // 5 MB
            format: 'csv',
            compressed: false
        );
        $this->assertSame('5 MB', $result->humanFileSize());
    }
    public function test_it_formats_file_size_in_gigabytes(): void
    {
        $result = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'test.csv',
            recordCount: 1,
            fileSize: 2 * 1024 * 1024 * 1024, // 2 GB
            format: 'csv',
            compressed: false
        );
        $this->assertSame('2 GB', $result->humanFileSize());
    }
    public function test_it_formats_file_size_in_terabytes(): void
    {
        $result = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'test.csv',
            recordCount: 1,
            fileSize: 3 * 1024 * 1024 * 1024 * 1024, // 3 TB
            format: 'csv',
            compressed: false
        );
        $this->assertSame('3 TB', $result->humanFileSize());
    }
    public function test_it_formats_file_size_with_decimal_precision(): void
    {
        $result = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'test.csv',
            recordCount: 1,
            fileSize: 1536, // 1.5 KB
            format: 'csv',
            compressed: false
        );
        $this->assertSame('1.5 KB', $result->humanFileSize());
    }
    public function test_it_handles_zero_file_size(): void
    {
        $result = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'test.csv',
            recordCount: 0,
            fileSize: 0,
            format: 'csv',
            compressed: false
        );
        $this->assertSame('0 B', $result->humanFileSize());
    }
    public function test_it_stores_compressed_flag_correctly(): void
    {
        $compressedResult = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'test.csv.zip',
            recordCount: 1,
            fileSize: 100,
            format: 'csv',
            compressed: true
        );
        $uncompressedResult = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'test.csv',
            recordCount: 1,
            fileSize: 100,
            format: 'csv',
            compressed: false
        );
        $this->assertTrue($compressedResult->compressed);
        $this->assertFalse($uncompressedResult->compressed);
    }
    public function test_it_stores_sql_format_correctly(): void
    {
        $result = new ArchiveResult(
            modelClass: TestPrunableModel::class,
            storagePath: 'test.sql',
            recordCount: 1,
            fileSize: 100,
            format: 'sql',
            compressed: false
        );
        $this->assertSame('sql', $result->format);
    }
}
