#include <doctest/doctest.h>

#include <archivepp/string.hpp>
#include <archivepp/extensions.hpp>
#include <archivepp/secure_allocator.hpp>

#include <cstring>
#include <string>

using secure_char_allocator = archivepp::secure_allocator<char>;
using secure_wchar_allocator = archivepp::secure_allocator<wchar_t>;

TEST_CASE("normalize_paths")
{
    archivepp::string path = ARCHIVEPP_STR("this\\path\\does\\not\\exist");

    REQUIRE(archivepp::normalize_path(path) == ARCHIVEPP_STR("this/path/does/not/exist"));
}

TEST_CASE("secure_allocator<char>")
{
    secure_char_allocator alloc;

    auto ptr = alloc.allocate(5);
    ptr[0] = 'H'; ptr[1] = 'E'; ptr[2] = 'L'; ptr[3] = 'L'; ptr[4] = 'O';

    archivepp::secure_scrub(ptr, 5);
    REQUIRE(std::strncmp(ptr, "\0\0\0\0\0", 5) == 0);

    alloc.destroy(ptr);
    alloc.deallocate(ptr, 5);
}

TEST_CASE("secure_allocator<wchar_t>")
{
    secure_wchar_allocator alloc;

    auto ptr = alloc.allocate(5);
    ptr[0] = L'H'; ptr[1] = L'E'; ptr[2] = L'L'; ptr[3] = L'L'; ptr[4] = L'O';

    archivepp::secure_scrub(ptr, 10);
    REQUIRE(std::wcsncmp(ptr, L"\0\0\0\0\0", 5) == 0);

    alloc.destroy(ptr);
    alloc.deallocate(ptr, 5);
}

TEST_CASE("zip add is_whitelisted remove extensions")
{
    archivepp::zip::add_extensions({ ARCHIVEPP_STR("test1"), ARCHIVEPP_STR("test2"), ARCHIVEPP_STR("test3") });
    REQUIRE(archivepp::zip::get_extensions().size() == 5); // already has 2

    REQUIRE(archivepp::zip::is_whitelisted(ARCHIVEPP_STR("test2")) == true);
    REQUIRE(archivepp::zip::is_whitelisted(ARCHIVEPP_STR("1234")) == false);

    archivepp::zip::remove_extensions({ ARCHIVEPP_STR("test1"), ARCHIVEPP_STR("test2") });
    REQUIRE(archivepp::zip::get_extensions().size() == 3);
}

TEST_CASE("rar add is_whitelisted remove extensions")
{
    archivepp::rar::add_extensions({ ARCHIVEPP_STR("test1"), ARCHIVEPP_STR("test2"), ARCHIVEPP_STR("test3") });
    REQUIRE(archivepp::rar::get_extensions().size() == 5); // already has 2

    REQUIRE(archivepp::rar::is_whitelisted(ARCHIVEPP_STR("test2")) == true);
    REQUIRE(archivepp::rar::is_whitelisted(ARCHIVEPP_STR("1234")) == false);

    archivepp::rar::remove_extensions({ ARCHIVEPP_STR("test1"), ARCHIVEPP_STR("test2") });
    REQUIRE(archivepp::rar::get_extensions().size() == 3);
}
