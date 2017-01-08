#include <doctest/doctest.h>

#include <archivepp/archive_zip.hpp>

static archivepp::string password = "test";

TEST_CASE("archive_zip - constructor on nonexistent file")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    REQUIRE(ec.value() == ZIP_ER_NOENT);
}

TEST_CASE("archive_zip - get path")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    REQUIRE(archive.get_path() == path);
}

TEST_CASE("archive_zip - get number of entries")
{
    archivepp::string path("../../tests/zip/archive.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    REQUIRE(ec.value() == 0);

    REQUIRE(archive.get_number_of_entries() == 5);
}

TEST_CASE("archive_rar - get number of entries on nonexistent file throws")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    CHECK_THROWS_AS(archive.get_number_of_entries(), archivepp::null_pointer_error);
}

TEST_CASE("archive_zip - get entries")
{
    archivepp::string path("../../tests/zip/archive.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries();
    
    REQUIRE(entries.size() == 5);
}

TEST_CASE("archive_zip - get contents by name")
{
    archivepp::string path("../../tests/zip/archive.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

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

TEST_CASE("archive_zip - get encrypted contents with archive password")
{
    archivepp::string path("../../tests/zip/archive_encrypted.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, password, ec);

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

TEST_CASE("archive_zip - get encrypted contents with archive_entry password")
{
    archivepp::string path("../../tests/zip/archive_encrypted.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

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
