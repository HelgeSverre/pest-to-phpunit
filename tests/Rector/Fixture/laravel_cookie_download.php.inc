<?php
uses(Tests\TestCase::class);

test('has cookie', function () {
    $response = $this->get('/');
    expect($response)->assertCookie('name', 'Taylor');
});

test('cookie missing', function () {
    $response = $this->get('/');
    expect($response)->assertCookieMissing('secret');
});

test('cookie expired', function () {
    $response = $this->get('/logout');
    expect($response)->assertCookieExpired('session');
});

test('file download', function () {
    $response = $this->get('/export');
    expect($response)->assertDownload('report.csv');
});

test('download without name', function () {
    $response = $this->get('/export');
    expect($response)->assertDownload();
});

-----
<?php

class LaravelCookieDownloadTest extends \Tests\TestCase
{
    public function test_has_cookie(): void
    {
        $response = $this->get('/');
        $response->assertCookie('name', 'Taylor');
    }
    public function test_cookie_missing(): void
    {
        $response = $this->get('/');
        $response->assertCookieMissing('secret');
    }
    public function test_cookie_expired(): void
    {
        $response = $this->get('/logout');
        $response->assertCookieExpired('session');
    }
    public function test_file_download(): void
    {
        $response = $this->get('/export');
        $response->assertDownload('report.csv');
    }
    public function test_download_without_name(): void
    {
        $response = $this->get('/export');
        $response->assertDownload();
    }
}
