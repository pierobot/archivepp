#pragma once

#include <archivepp/basic_archive_entry.hpp>

#include <zip.h>

namespace archivepp
{
    class archive_entry_zip final : public basic_archive_entry
    {
    public:
        archive_entry_zip(archivepp::string name, uint64_t index, uint64_t uncompressed_size, uint64_t compressed_size, std::error_code & ec);

        virtual ~archive_entry_zip();
    protected:
    private:
    };
}
