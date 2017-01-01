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
    archivepp::string path("../../tests/zip/archive.zip");
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
    archivepp::string path("../../tests/zip/archive.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    auto entries = archive.get_entries();
    
    REQUIRE(entries.size() == 5);
}

TEST_CASE("archive_entry_zip - get name")
{
    archivepp::string path("../../tests/zip/archive.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    auto entries = archive.get_entries();
    
    REQUIRE(entries[0]->get_name() == "archive/");
    REQUIRE(entries[2]->get_name() == "archive/2");
    REQUIRE(entries[4]->get_name() == "archive/empty/");
}

TEST_CASE("archive_entry_zip - get size")
{
    archivepp::string path("../../tests/zip/archive.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    auto entries = archive.get_entries();

    REQUIRE(entries[1]->get_size() == 1);
    REQUIRE(entries[2]->get_size() == 2);
    REQUIRE(entries[3]->get_size() == 3);
}

TEST_CASE("archive_entry_zip - get contents")
{
    archivepp::string path("../../tests/zip/archive.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, ec);

    auto entries = archive.get_entries();

    REQUIRE(entries[1]->get_contents() == "1");
    REQUIRE(entries[2]->get_contents() == "22");
    REQUIRE(entries[3]->get_contents() == "333");
}
