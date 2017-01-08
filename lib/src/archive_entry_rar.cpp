#include <archivepp/archive_entry_rar.hpp>

namespace archivepp
{
    archive_entry_rar::archive_entry_rar(archivepp::string name,
                                         uint64_t index,
                                         uint64_t uncompressed_size,
                                         uint64_t compressed_size,
                                         std::error_code & ec) :
        basic_archive_entry(std::move(name), index, uncompressed_size, compressed_size)
    {
    }

    archive_entry_rar::~archive_entry_rar()
    {
    }
}
