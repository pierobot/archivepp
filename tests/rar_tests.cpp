#include <doctest/doctest.h>

#include <archivepp/archive_rar.hpp>

static archivepp::string nonexistent_path = ARCHIVEPP_STR("../../doesnotexist");
static archivepp::string archive_path = ARCHIVEPP_STR("../../tests/rar/archive.rar");
static archivepp::string archive_encrypted_path = ARCHIVEPP_STR("../../tests/rar/archive_encrypted.rar");
static archivepp::string archive_file_1 = ARCHIVEPP_STR("archive/1");
static archivepp::string archive_file_2 = ARCHIVEPP_STR("archive/2");
static archivepp::string archive_file_3 = ARCHIVEPP_STR("archive/3");
static archivepp::string archive_encrypted_file_1 = ARCHIVEPP_STR("archive_encrypted/1");
static archivepp::string archive_encrypted_file_2 = ARCHIVEPP_STR("archive_encrypted/2");
static archivepp::string archive_encrypted_file_3 = ARCHIVEPP_STR("archive_encrypted/3");
static archivepp::secure_string password = ARCHIVEPP_STR("test");

TEST_CASE("archive_rar - constructor on nonexistent file")
{
    archivepp::string path(nonexistent_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == ERAR_EOPEN);
}

TEST_CASE("archive_rar - get path")
{
    archivepp::string path(nonexistent_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(archive.get_path() == path);
}

TEST_CASE("archive_rar - get number of entries")
{
    archivepp::string path(archive_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    REQUIRE(archive.get_number_of_entries() == 5);
}

TEST_CASE("archive_rar - get number of entries on nonexistent file throws")
{
    archivepp::string path(nonexistent_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    CHECK_THROWS_AS(archive.get_number_of_entries(), archivepp::null_pointer_error);
}

TEST_CASE("archive_rar - get entries")
{
    archivepp::string path(archive_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries();
    
    REQUIRE(entries.size() == 5);
}

TEST_CASE("archive_rar - get contents by name")
{
    archivepp::string path(archive_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    std::string contents = archive.get_contents(archive_file_2, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "22");

    contents = archive.get_contents(archive_file_1, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "1");

    contents = archive.get_contents(archive_file_3, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "333");
}

TEST_CASE("archive_rar - get encrypted contents with archive password")
{
    archivepp::string path(archive_encrypted_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, password, ec);

    REQUIRE(ec.value() == 0);

    std::string contents = archive.get_contents(archive_encrypted_file_2, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "22");

    contents = archive.get_contents(archive_encrypted_file_1, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "1");

    contents = archive.get_contents(archive_encrypted_file_3, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "333");
}

TEST_CASE("archive_rar - get encrypted contents with archive_entry password")
{
    archivepp::string path(archive_encrypted_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    std::string contents = archive.get_contents(archive_encrypted_file_2, password, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "22");

    contents = archive.get_contents(archive_encrypted_file_1, password, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "1");

    contents = archive.get_contents(archive_encrypted_file_3, password, ec);
    REQUIRE(ec.value() == 0);
    REQUIRE(contents == "333");
}

TEST_CASE("archive_rar - get entries and get contents")
{
    archivepp::string path(archive_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != archive_file_3;
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == archive_file_3);
    REQUIRE(entries[0]->get_uncompressed_size() == 3);

    std::string contents = archive.get_contents(entries[0], ec);
    REQUIRE(contents == "333");

    entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != archive_file_2;
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == archive_file_2);
    REQUIRE(entries[0]->get_uncompressed_size() == 2);

    contents = archive.get_contents(entries[0], ec);
    REQUIRE(contents == "22");

    entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != archive_file_1;
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == archive_file_1);
    REQUIRE(entries[0]->get_uncompressed_size() == 1);

    contents = archive.get_contents(entries[0], ec);
    REQUIRE(contents == "1");
}

TEST_CASE("archive_rar - get entries and get encrypted contents with password")
{
    archivepp::string path(archive_encrypted_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    auto entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != archive_encrypted_file_3;
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == archive_encrypted_file_3);
    REQUIRE(entries[0]->get_uncompressed_size() == 3);

    std::string contents = archive.get_contents(entries[0], password, ec);
    REQUIRE(contents == "333");

    entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != archive_encrypted_file_2;
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == archive_encrypted_file_2);
    REQUIRE(entries[0]->get_uncompressed_size() == 2);
    contents = archive.get_contents(entries[0], password, ec);
    REQUIRE(contents == "22");

    entries = archive.get_entries([](archivepp::archive::entry_pointer const & entry_ptr)
    {
        return entry_ptr->get_name() != archive_encrypted_file_1;
    });

    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0]->get_name() == archive_encrypted_file_1);
    REQUIRE(entries[0]->get_uncompressed_size() == 1);

    contents = archive.get_contents(entries[0], password, ec);
    REQUIRE(contents == "1");
}

TEST_CASE("archive_rar - get contents to invalid entry_pointer throws")
{
    archivepp::string path(archive_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    REQUIRE(ec.value() == 0);

    archivepp::archive::entry_pointer entry_ptr(nullptr);
    CHECK_THROWS_AS(archive.get_contents(entry_ptr, ec), archivepp::null_argument_error);
}

TEST_CASE("archive_rar - get entries to nonexistent file throws")
{
    archivepp::string path(nonexistent_path);
    std::error_code ec;
    archivepp::archive_rar archive(path, ec);

    CHECK_THROWS_AS(archive.get_entries(), archivepp::null_pointer_error);
}
