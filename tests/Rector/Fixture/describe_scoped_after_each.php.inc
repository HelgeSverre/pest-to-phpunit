<?php

describe('Connection', function () {
    afterEach(function () {
        $this->connection->close();
    });

    test('connects', function () {
        $this->connection = connect();
        expect($this->connection)->toBeInstanceOf(Connection::class);
    });
});

-----
<?php

class DescribeScopedAfterEachTest extends \PHPUnit\Framework\TestCase
{
    protected $connection;
    public function test_connection_connects(): void
    {
        try {
            $this->connection = connect();
            $this->assertInstanceOf(Connection::class, $this->connection);
        } finally {
            $this->connection->close();
        }
    }
}
