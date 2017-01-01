#include <doctest/doctest.h>

#include <archivepp/archive_zip.hpp>

TEST_CASE("archive_zip - constructor on nonexistent file")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    REQUIRE(ec.value() == ZIP_ER_NOENT);

    REQUIRE(archive.get_path() == path);
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

TEST_CASE("archive_zip - get entries")
{
    archivepp::string path("../../tests/zip/get_number_of_entries.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    auto entries = archive.get_entries();
    
    REQUIRE(entries.size() == 5);
}

TEST_CASE("archive_entry_zip - get name")
{
    archivepp::string path("../../tests/zip/get_number_of_entries.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    auto entries = archive.get_entries();
    
    REQUIRE(entries[0]->get_name() == "get_number_of_entries/");
    REQUIRE(entries[2]->get_name() == "get_number_of_entries/2");
    REQUIRE(entries[4]->get_name() == "get_number_of_entries/empty/");
}