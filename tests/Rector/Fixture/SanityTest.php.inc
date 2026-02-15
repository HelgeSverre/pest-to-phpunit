<?php

use HelgeSverre\Telefonkatalog\DataSources\Gulesider;
use HelgeSverre\Telefonkatalog\DataSources\Opplysningen1881;
use HelgeSverre\Telefonkatalog\DataSources\Opplysningen1890;

it('can search Gulesider', function () {
    $lookup = new Gulesider;
    $person = $lookup->find('95965871');

    expect($person)->not()->toBeNull()
        ->and($person->name)->toBe('Helge Sverre Hessevik Liseth')
        ->and($person->address)->toBe('Vognstølen 29')
        ->and($person->city)->toBe('Bergen')
        ->and($person->postalCode)->toBe('5096')
        ->and($person->phone)->toBe('95965871')
        ->and($person->url)->toBe('https://www.gulesider.no/helge+sverre+hessevik+liseth+bergen/77190505/person');
});

it('can search Gulesider by name', function () {
    $lookup = new Gulesider;
    $people = $lookup->search('helge sverre liseth');

    expect($people)->toBeCollection()->and($people)->toHaveCount(1);
});

it('can search Opplysningen 1881', function () {
    $lookup = new Opplysningen1881;
    $person = $lookup->find('95965871');

    expect($person)->not()->toBeNull()
        ->and($person->name)->toBe('Helge Sverre Hessevik Liseth')
        ->and($person->address)->toBe('Vognstølen 29')
        ->and($person->city)->toBe('Bergen')
        ->and($person->postalCode)->toBe('5096')
        ->and($person->phone)->toBe('95965871')
        ->and($person->url)->toBe('https://www.1881.no/person/bergen/bergen/helge-sverre-hessevik-liseth_12982286S2/?query=95965871');
});

it('can search Opplysningen 1881 by name', function () {
    $lookup = new Opplysningen1881;
    $people = $lookup->search('helge sverre liseth');

    expect($people)->toBeCollection()->and($people)->isNotEmpty();
});

it('can search Opplysningen 1890', function () {
    $lookup = new Opplysningen1890;
    $person = $lookup->find('95965871');

    expect($person)->not()->toBeNull()
        ->and($person->name)->toBe('Helge Sverre Hessevik Liseth')
        ->and($person->address)->toBe('Vognstølen 29')
        ->and($person->city)->toBe('Bergen')
        ->and($person->postalCode)->toBe('5096')
        ->and($person->phone)->toBe('95965871')
        ->and($person->url)->toBe('https://1890.no/id/Helge-Sverre-Hessevik-Liseth-5096');
});

it('can search Opplysningen 1890 by name', function () {
    $lookup = new Opplysningen1890;
    $people = $lookup->search('helge sverre liseth');

    expect($people)->toBeCollection()->and($people)->toHaveCount(1);
});

// Edge case tests for empty/non-existent results

it('returns null for non-existent phone number on Gulesider', function () {
    $lookup = new Gulesider;
    $person = $lookup->find('00000000');

    expect($person)->toBeNull();
});

it('returns empty collection for non-existent name on Gulesider', function () {
    $lookup = new Gulesider;
    $results = $lookup->search('xyznonexistentperson12345');

    expect($results)->toBeCollection()->and($results)->toBeEmpty();
});

it('returns null for non-existent phone number on Opplysningen 1881', function () {
    $lookup = new Opplysningen1881;
    $person = $lookup->find('00000000');

    expect($person)->toBeNull();
});

it('returns empty collection for non-existent name on Opplysningen 1881', function () {
    $lookup = new Opplysningen1881;
    $results = $lookup->search('xyznonexistentperson12345');

    expect($results)->toBeCollection()->and($results)->toBeEmpty();
});

it('handles non-matching queries gracefully on Opplysningen 1890', function () {
    $lookup = new Opplysningen1890;
    // 1890.no does aggressive fuzzy matching, results are unpredictable
    // Just verify it returns valid type without throwing
    $person = $lookup->find('zzzzzzzzzzzzz');

    expect($person === null || $person instanceof \HelgeSverre\Telefonkatalog\Data\Person)->toBeTrue();
});

it('returns empty collection for non-existent name on Opplysningen 1890', function () {
    $lookup = new Opplysningen1890;
    $results = $lookup->search('xyznonexistentperson12345');

    expect($results)->toBeCollection()->and($results)->toBeEmpty();
});

-----
<?php

use HelgeSverre\Telefonkatalog\DataSources\Gulesider;
use HelgeSverre\Telefonkatalog\DataSources\Opplysningen1881;
use HelgeSverre\Telefonkatalog\DataSources\Opplysningen1890;
class SanityTest extends \PHPUnit\Framework\TestCase
{
    public function test_it_can_search_gulesider(): void
    {
        $lookup = new Gulesider;
        $person = $lookup->find('95965871');
        $this->assertNotNull($person);
        $this->assertSame('Helge Sverre Hessevik Liseth', $person->name);
        $this->assertSame('Vognstølen 29', $person->address);
        $this->assertSame('Bergen', $person->city);
        $this->assertSame('5096', $person->postalCode);
        $this->assertSame('95965871', $person->phone);
        $this->assertSame('https://www.gulesider.no/helge+sverre+hessevik+liseth+bergen/77190505/person', $person->url);
    }
    public function test_it_can_search_gulesider_by_name(): void
    {
        $lookup = new Gulesider;
        $people = $lookup->search('helge sverre liseth');
        $this->assertInstanceOf(\Illuminate\Support\Collection::class, $people);
        $this->assertCount(1, $people);
    }
    public function test_it_can_search_opplysningen_1881(): void
    {
        $lookup = new Opplysningen1881;
        $person = $lookup->find('95965871');
        $this->assertNotNull($person);
        $this->assertSame('Helge Sverre Hessevik Liseth', $person->name);
        $this->assertSame('Vognstølen 29', $person->address);
        $this->assertSame('Bergen', $person->city);
        $this->assertSame('5096', $person->postalCode);
        $this->assertSame('95965871', $person->phone);
        $this->assertSame('https://www.1881.no/person/bergen/bergen/helge-sverre-hessevik-liseth_12982286S2/?query=95965871', $person->url);
    }
    public function test_it_can_search_opplysningen_1881_by_name(): void
    {
        $lookup = new Opplysningen1881;
        $people = $lookup->search('helge sverre liseth');
        $this->assertInstanceOf(\Illuminate\Support\Collection::class, $people);
    }
    public function test_it_can_search_opplysningen_1890(): void
    {
        $lookup = new Opplysningen1890;
        $person = $lookup->find('95965871');
        $this->assertNotNull($person);
        $this->assertSame('Helge Sverre Hessevik Liseth', $person->name);
        $this->assertSame('Vognstølen 29', $person->address);
        $this->assertSame('Bergen', $person->city);
        $this->assertSame('5096', $person->postalCode);
        $this->assertSame('95965871', $person->phone);
        $this->assertSame('https://1890.no/id/Helge-Sverre-Hessevik-Liseth-5096', $person->url);
    }
    public function test_it_can_search_opplysningen_1890_by_name(): void
    {
        $lookup = new Opplysningen1890;
        $people = $lookup->search('helge sverre liseth');
        $this->assertInstanceOf(\Illuminate\Support\Collection::class, $people);
        $this->assertCount(1, $people);
    }
    public function test_it_returns_null_for_nonexistent_phone_number_on_gulesider(): void
    {
        $lookup = new Gulesider;
        $person = $lookup->find('00000000');
        $this->assertNull($person);
    }
    public function test_it_returns_empty_collection_for_nonexistent_name_on_gulesider(): void
    {
        $lookup = new Gulesider;
        $results = $lookup->search('xyznonexistentperson12345');
        $this->assertInstanceOf(\Illuminate\Support\Collection::class, $results);
        $this->assertEmpty($results);
    }
    public function test_it_returns_null_for_nonexistent_phone_number_on_opplysningen_1881(): void
    {
        $lookup = new Opplysningen1881;
        $person = $lookup->find('00000000');
        $this->assertNull($person);
    }
    public function test_it_returns_empty_collection_for_nonexistent_name_on_opplysningen_1881(): void
    {
        $lookup = new Opplysningen1881;
        $results = $lookup->search('xyznonexistentperson12345');
        $this->assertInstanceOf(\Illuminate\Support\Collection::class, $results);
        $this->assertEmpty($results);
    }
    public function test_it_handles_nonmatching_queries_gracefully_on_opplysningen_1890(): void
    {
        $lookup = new Opplysningen1890;
        // 1890.no does aggressive fuzzy matching, results are unpredictable
        // Just verify it returns valid type without throwing
        $person = $lookup->find('zzzzzzzzzzzzz');
        $this->assertTrue($person === null || $person instanceof \HelgeSverre\Telefonkatalog\Data\Person);
    }
    public function test_it_returns_empty_collection_for_nonexistent_name_on_opplysningen_1890(): void
    {
        $lookup = new Opplysningen1890;
        $results = $lookup->search('xyznonexistentperson12345');
        $this->assertInstanceOf(\Illuminate\Support\Collection::class, $results);
        $this->assertEmpty($results);
    }
}
