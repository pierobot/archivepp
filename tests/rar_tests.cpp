#include <doctest/doctest.h>

#include <archivepp/archive_rar.hpp>

static std::string const nopassword;

TEST_CASE("archive_rar - constructor on nonexistent file")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_rar archive(path, nopassword, ec);

    REQUIRE(ec.value() == ERAR_EOPEN);
}

TEST_CASE("archive_rar - get path")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_rar archive(path, nopassword, ec);

    REQUIRE(archive.get_path() == path);
}

TEST_CASE("archive_rar - get number of entries")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, nopassword, ec);

    REQUIRE(ec.value() == 0);

    REQUIRE(archive.get_number_of_entries() == 5);
}

TEST_CASE("archive_rar - get number of entries on nonexistent file throws")
{
    archivepp::string path("../../doesnotexist");
    std::error_code ec;
    archivepp::archive_rar archive(path, nopassword, ec);

    CHECK_THROWS_AS(archive.get_number_of_entries(), archivepp::null_pointer_error);
}

TEST_CASE("archive_rar - get entries")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, nopassword, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries();
    
    REQUIRE(entries.size() == 5);
}

/*

TEST_CASE("archive_entry_rar - get name")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries();
    
    REQUIRE(entries[0]->get_name() == "archive/3");
    REQUIRE(entries[2]->get_name() == "archive/2");
    REQUIRE(entries[4]->get_name() == "archive/empty/");
}

TEST_CASE("archive_entry_rar - get index")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries();

    REQUIRE(entries[1]->get_index() == 1);
    REQUIRE(entries[2]->get_index() == 2);
    REQUIRE(entries[3]->get_index() == 3);
}

TEST_CASE("archive_entry_rar - get size")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries();

    REQUIRE(entries[1]->get_size() == 1);
    REQUIRE(entries[2]->get_size() == 2);
    REQUIRE(entries[3]->get_size() == 3);
}

TEST_CASE("archive_entry_rar - get contents")
{
    archivepp::string path("../../tests/rar/archive.rar");
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries();

    REQUIRE(entries[1]->get_contents(ec) == "1");
    REQUIRE(ec.value() == 0);
    REQUIRE(entries[2]->get_contents(ec) == "22");
    REQUIRE(ec.value() == 0);
    REQUIRE(entries[3]->get_contents(ec) == "333");
    REQUIRE(ec.value() == 0);
}
*/

// TEST_CASE("archive_entry_rar - get encrypted contents without password")
// {
//     //archivepp::string path("../../tests/rar/archive_encrypted.rar");
//     archivepp::string path("/home/pierobot/github/archivepp/tests/rar/archive_encrypted.rar");
//     std::error_code ec;
//     archivepp::archive_rar archive(path, ec);

//     REQUIRE(ec.value() == 0);

//     auto entries = archive.get_entries();
    
//     std::string contents = entries[1]->get_contents(ec);
//     REQUIRE(ec.value() == ERAR_MISSING_PASSWORD);
        
//     contents = entries[2]->get_contents(ec);
//     REQUIRE(ec.value() == ERAR_MISSING_PASSWORD);
        
//     contents = entries[3]->get_contents(ec);
//     REQUIRE(ec.value() == ERAR_MISSING_PASSWORD);
// }
