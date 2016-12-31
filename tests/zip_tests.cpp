#include <doctest/doctest.h>

#include <archivepp/archive_zip.hpp>

TEST_CASE("constructor on nonexistent file")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    REQUIRE(ec.value() == ZIP_ER_NOENT);

    REQUIRE(archive.get_path() == path);
}

TEST_CASE("method get_path")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    REQUIRE(archive.get_path() == path);
}

TEST_CASE("method get_number_of_entries")
{
    archivepp::string path("../../tests/zip/get_number_of_entries.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    REQUIRE(ec.value() == 0);

    /*
        Entries:
        get_number_of_entries (1)
        - 1                   (2)
        - 2                   (3)
        - 3                   (4)
        - empty               (5)
    */
    REQUIRE(archive.get_number_of_entries() == 5);
}
