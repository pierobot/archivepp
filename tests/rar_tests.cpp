#include <doctest/doctest.h>

#include <archivepp/archive_rar.hpp>

static archivepp::string password = "test";

TEST_CASE("archive_rar - constructor on nonexistent file")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == ERAR_EOPEN);
}

TEST_CASE("archive_rar - get path")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(archive.get_path() == path);
}

TEST_CASE("archive_rar - get number of entries")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    REQUIRE(archive.get_number_of_entries() == 5);
}

TEST_CASE("archive_rar - get number of entries on nonexistent file throws")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    CHECK_THROWS_AS(archive.get_number_of_entries(), archivepp::null_pointer_error);
}

TEST_CASE("archive_rar - get entries")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries();
    
    REQUIRE(entries.size() == 5);
}

TEST_CASE("archive_rar - get contents by name")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    std::string contents = archive.get_contents("archive/2", ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "22");

    contents = archive.get_contents("archive/1", ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "1");

    contents = archive.get_contents("archive/3", ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "333");
}

TEST_CASE("archive_rar - get encrypted contents with archive password")
{
    archivepp::string path("../../tests/rar/archive_encrypted.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, password, ec);

    REQUIRE(ec.value() == 0);

    std::string contents = archive.get_contents("archive_encrypted/2", ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "22");

    contents = archive.get_contents("archive_encrypted/1", ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "1");

    contents = archive.get_contents("archive_encrypted/3", ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "333");
}

TEST_CASE("archive_rar - get encrypted contents with archive_entry password")
{
    archivepp::string path("../../tests/rar/archive_encrypted.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    std::string contents = archive.get_contents("archive_encrypted/2", password, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "22");

    contents = archive.get_contents("archive_encrypted/1", password, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "1");

    contents = archive.get_contents("archive_encrypted/3", password, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "333");
}

TEST_CASE("archive_rar - get entries and get contents")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != "archive/3";
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == "archive/3");
    REQUIRE(entries[0]->get_uncompressed_size() == 3);

    std::string contents = archive.get_contents(entries[0], ec);
    REQUIRE(contents == "333");

    entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != "archive/2";
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == "archive/2");
    REQUIRE(entries[0]->get_uncompressed_size() == 2);

    contents = archive.get_contents(entries[0], ec);
    REQUIRE(contents == "22");

    entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != "archive/1";
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == "archive/1");
    REQUIRE(entries[0]->get_uncompressed_size() == 1);

    contents = archive.get_contents(entries[0], ec);
    REQUIRE(contents == "1");
}

TEST_CASE("archive_rar - get entries and get encrypted contents with password")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != "archive/3";
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == "archive/3");
    REQUIRE(entries[0]->get_uncompressed_size() == 3);

    std::string contents = archive.get_contents(entries[0], password, ec);
    REQUIRE(contents == "333");

    entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != "archive/2";
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == "archive/2");
    REQUIRE(entries[0]->get_uncompressed_size() == 2);
    contents = archive.get_contents(entries[0], password, ec);
    REQUIRE(contents == "22");

    entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != "archive/1";
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == "archive/1");
    REQUIRE(entries[0]->get_uncompressed_size() == 1);

    contents = archive.get_contents(entries[0], password, ec);
    REQUIRE(contents == "1");
}

TEST_CASE("archive_rar - get contents to invalid entry_pointer throws")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    archivepp::archive::entry_pointer entry_ptr(nullptr);
    CHECK_THROWS_AS(archive.get_contents(entry_ptr, ec), archivepp::null_argument_error);   
}
