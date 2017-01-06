#include <doctest/doctest.h>

#include <archivepp/archive_zip.hpp>

static archivepp::string nopassword;

TEST_CASE("archive_zip - constructor on nonexistent file")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_zip archive(path, nopassword, ec);

    REQUIRE(ec.value() == ZIP_ER_NOENT);
}

TEST_CASE("archive_zip - get path")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_zip archive(path, nopassword, ec);

    REQUIRE(archive.get_path() == path);
}

TEST_CASE("archive_zip - get number of entries")
{
    archivepp::string path("../../tests/zip/archive.zip");
    std::error_code ec;
    archivepp::archive_zip archive(path, nopassword, ec);

    REQUIRE(ec.value() == 0);

    REQUIRE(archive.get_number_of_entries() == 5);
}

TEST_CASE("archive_rar - get number of entries on nonexistent file throws")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_zip archive(path, nopassword, ec);

    CHECK_THROWS_AS(archive.get_number_of_entries(), archivepp::null_pointer_error);
}

// TEST_CASE("archive_zip - get entries")
// {
//     archivepp::string path("../../tests/zip/archive.zip");
//     std::error_code ec;
//     archivepp::archive_zip archive(path, nopassword, ec);

//     REQUIRE(ec.value() == 0);

//     auto entries = archive.get_entries();
    
//     REQUIRE(entries.size() == 5);
// }

// TEST_CASE("archive_entry_zip - get name")
// {
//     archivepp::string path("../../tests/zip/archive.zip");
//     std::error_code ec;
//     archivepp::archive_zip archive(path, ec);

//     REQUIRE(ec.value() == 0);

//     auto entries = archive.get_entries();
    
//     REQUIRE(entries[0]->get_name() == "archive/");
//     REQUIRE(entries[2]->get_name() == "archive/2");
//     REQUIRE(entries[4]->get_name() == "archive/empty/");
// }

// TEST_CASE("archive_entry_zip - get index")
// {
//     archivepp::string path("../../tests/zip/archive.zip");
//     std::error_code ec;
//     archivepp::archive_zip archive(path, ec);

//     REQUIRE(ec.value() == 0);

//     auto entries = archive.get_entries();

//     REQUIRE(entries[1]->get_index() == 1);
//     REQUIRE(entries[2]->get_index() == 2);
//     REQUIRE(entries[3]->get_index() == 3);
// }

// TEST_CASE("archive_entry_zip - get size")
// {
//     archivepp::string path("../../tests/zip/archive.zip");
//     std::error_code ec;
//     archivepp::archive_zip archive(path, ec);

//     REQUIRE(ec.value() == 0);

//     auto entries = archive.get_entries();

//     REQUIRE(entries[1]->get_size() == 1);
//     REQUIRE(entries[2]->get_size() == 2);
//     REQUIRE(entries[3]->get_size() == 3);
// }

// TEST_CASE("archive_entry_zip - get contents")
// {
//     archivepp::string path("../../tests/zip/archive.zip");
//     std::error_code ec;
//     archivepp::archive_zip archive(path, ec);

//     REQUIRE(ec.value() == 0);

//     auto entries = archive.get_entries();

//     std::string contents = entries[1]->get_contents(ec);
//     REQUIRE(ec.value() == 0);
//     REQUIRE(contents == "1");
    
//     contents = entries[2]->get_contents(ec);
//     REQUIRE(ec.value() == 0);
//     REQUIRE(contents == "22");
    
//     contents = entries[3]->get_contents(ec);
//     REQUIRE(ec.value() == 0);
//     REQUIRE(contents == "333");
// }

// TEST_CASE("archive_entry_zip - get encrypted contents without password")
// {
//     archivepp::string path("../../tests/zip/archive_encrypted.zip");
//     std::error_code ec;
//     archivepp::archive_zip archive(path, ec);
    
//     REQUIRE(ec.value() == 0);

//     auto entries = archive.get_entries();
    
//     std::string contents = entries[1]->get_contents(ec);
//     REQUIRE(ec.value() == ZIP_ER_NOPASSWD);
        
//     contents = entries[2]->get_contents(ec);
//     REQUIRE(ec.value() == ZIP_ER_NOPASSWD);
        
//     contents = entries[3]->get_contents(ec);
//     REQUIRE(ec.value() == ZIP_ER_NOPASSWD);
// }

// TEST_CASE("archive_entry_zip - get encrypted contents with password")
// {
//     archivepp::string path("../../tests/zip/archive_encrypted.zip");
//     std::error_code ec;
//     archivepp::archive_zip archive(path, "test", ec);

//     auto entries = archive.get_entries();

//     std::string contents = entries[1]->get_contents(ec);
//     REQUIRE(ec.value() == 0);
//     REQUIRE(contents == "1");
    
//     contents = entries[2]->get_contents(ec);
//     REQUIRE(ec.value() == 0);
//     REQUIRE(contents == "22");
    
//     contents = entries[3]->get_contents(ec);
//     REQUIRE(ec.value() == 0);
//     REQUIRE(contents == "22");
// }
