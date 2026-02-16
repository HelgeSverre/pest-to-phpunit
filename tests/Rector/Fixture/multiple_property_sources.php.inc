<?php

beforeEach(function () {
    $this->client = new HttpClient();
});

afterEach(function () {
    $this->client = null;
});

test('makes request', function () {
    $this->response = $this->client->get('/');
    expect($this->response)->not->toBeNull();
});

-----
<?php

class MultiplePropertySourcesTest extends \PHPUnit\Framework\TestCase
{
    protected $client;
    protected $response;
    protected function setUp(): void
    {
        parent::setUp();
        $this->client = new HttpClient();
    }
    protected function tearDown(): void
    {
        parent::tearDown();
        $this->client = null;
    }
    public function test_makes_request(): void
    {
        $this->response = $this->client->get('/');
        $this->assertNotNull($this->response);
    }
}
